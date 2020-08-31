#include <onix/keyboard.h>
#include <onix/keymap.h>
#include <onix/stdio.h>
#include <onix/interrupt.h>
#include <onix/io.h>
#include <onix/assert.h>
#include <onix/console.h>

Queue keyqueue;
char key_buffer[KEYBOARD_BUFFER_SIZE];

static int STATUS_E0 = 0;
static int STATUS_SHIFT_L;
static int STATUS_SHIFT_R;
static int STATUS_ALT_L;
static int STATUS_ALT_R;
static int STATUS_CTRL_L;
static int STATUS_CTRL_R;
static int STATUS_CAPS_LOCK;
static int STATUS_NUM_LOCK;
static int STATUS_SCROLL_LOCK;

void keyboard_handler(int irq)
{
    int status = in_byte(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (!(status & 0x01))
        return;

    int code = in_byte(KEYBOARD_DATA_PORT);
    // kprintf("handler %x \n", code);
    enqueue(&keyqueue, &code);
    assert(keyqueue.count >= 0);
}

void init_keyboard()
{
    init_queue(&keyqueue, key_buffer, KEYBOARD_BUFFER_SIZE, sizeof(char));

    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}

char read_code()
{
    char code;
    int success = dequeue(&keyqueue, &code);
    if (success)
        return code;
    return 0;
}

int read_key()
{
    char code;
    bool make;
    u32 *keyrow;
    u32 key = 0;

    STATUS_E0 = 0;

    code = read_code();

    if (code == 0xE1)
    {
        int i;
        u8 pausebreak_codes[] = {0xE1, 0x1D, 0x45,
                                 0xE1, 0x9D, 0xC5};
        int pausebreak = 1;
        for (i = 1; i < 6; i++)
        {
            if (read_code() != pausebreak_codes[i])
            {
                pausebreak = 0;
                break;
            }
        }
        if (pausebreak)
        {
            key = PAUSEBREAK;
        }
    }
    else if (code == 0xE0)
    {
        code = read_code();

        /* PrintScreen down */
        if (code == 0x2A)
        {
            if (read_code() == 0xE0)
            {
                if (read_code() == 0x37)
                {
                    key = PRINTSCREEN;
                    make = 1;
                }
            }
        }
        /* PrintScreen up */
        else if (code == 0xB7)
        {
            if (read_code() == 0xE0)
            {
                if (read_code() == 0xAA)
                {
                    key = PRINTSCREEN;
                    make = 0;
                }
            }
        }
        if (key == 0)
        {
            STATUS_E0 = 1;
        }
    }

    if ((key != PAUSEBREAK) && (key != PRINTSCREEN))
    {
        make = code & FLAG_BREAK ? false : true;
        int index = (code & 0x7F) * MAP_COLS;
        u32 *keyrow = &keymap[index];

        int column = 0;
        if (STATUS_SHIFT_L || STATUS_SHIFT_R)
        {
            column = 1;
        }
        if (STATUS_E0)
        {
            column = 2;
            STATUS_E0 = 0;
        }

        u32 key = keyrow[column];
        switch (key)
        {
        case SHIFT_L:
            STATUS_SHIFT_L = make;
            key = 0;
            break;
        case SHIFT_R:
            STATUS_SHIFT_R = make;
            key = 0;
            break;
        case CTRL_L:
            STATUS_CTRL_L = make;
            key = 0;
            break;
        case CTRL_R:
            STATUS_CTRL_R = make;
            key = 0;
            break;
        case ALT_L:
            STATUS_ALT_L = make;
            key = 0;
            break;
        case ALT_R:
            STATUS_ALT_R = make;
            key = 0;
            break;
        default:
            break;
        }
        if (make)
        {
            key |= STATUS_SHIFT_L ? FLAG_SHIFT_L : 0;
            key |= STATUS_SHIFT_R ? FLAG_SHIFT_R : 0;
            key |= STATUS_CTRL_L ? FLAG_CTRL_L : 0;
            key |= STATUS_CTRL_R ? FLAG_CTRL_R : 0;
            key |= STATUS_ALT_L ? FLAG_ALT_L : 0;
            key |= STATUS_ALT_R ? FLAG_ALT_R : 0;
            return key;
        }
        return NULL;
    }
}

void read_keyboard(callback handler)
{
    if (keyqueue.count <= 0)
        return;

    int key = read_key();
    if (key == NULL)
        return;
    handler(key);
}