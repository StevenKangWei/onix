#include <onix/keyboard.h>
#include <onix/keymap.h>
#include <onix/stdio.h>
#include <onix/interrupt.h>
#include <onix/io.h>
#include <onix/assert.h>
#include <onix/console.h>

KeyBoard keyboard;

static void keyboard_set_leds();
static void keyboard_wait();
static void keyboard_ack();

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
    assert(irq == 1);
    int status = in_byte(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (!(status & 0x01))
        return;

    int code = in_byte(KEYBOARD_DATA_PORT);
    // kprintf("handler %x \n", code);
    enqueue(&keyboard.queue, &code);
    assert(keyboard.queue.count >= 0);
}

void init_keyboard()
{
    STATUS_SHIFT_L = 0;
    STATUS_SHIFT_R = 0;
    STATUS_ALT_L = 0;
    STATUS_ALT_R = 0;
    STATUS_CTRL_L = 0;
    STATUS_CTRL_R = 0;
    STATUS_CAPS_LOCK = 0;
    STATUS_NUM_LOCK = 1;
    STATUS_SCROLL_LOCK = 0;

    init_queue(&keyboard.queue, keyboard.buffer, KEYBOARD_BUFFER_SIZE, sizeof(char));

    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}

char read_code()
{
    char code;
    int success = dequeue(&keyboard.queue, &code);
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
            key = KEY_PAUSEBREAK;
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
                    key = KEY_PRINTSCREEN;
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
                    key = KEY_PRINTSCREEN;
                    make = 0;
                }
            }
        }
        if (key == 0)
        {
            STATUS_E0 = 1;
        }
    }

    if ((key != KEY_PAUSEBREAK) && (key != KEY_PRINTSCREEN))
    {
        make = code & FLAG_BREAK ? false : true;
        int index = (code & 0x7F) * MAP_COLS;
        u32 *keyrow = &keymap[index];

        int column = 0;
        int caps = STATUS_SHIFT_L || STATUS_SHIFT_R;
        if (STATUS_CAPS_LOCK && (keyrow[0] >= 'a') && (keyrow[0] <= 'z'))
        {
            caps = !caps;
        }
        if (caps)
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
        case KEY_SHIFT_L:
            STATUS_SHIFT_L = make;
            key = 0;
            break;
        case KEY_SHIFT_R:
            STATUS_SHIFT_R = make;
            key = 0;
            break;
        case KEY_CTRL_L:
            STATUS_CTRL_L = make;
            key = 0;
            break;
        case KEY_CTRL_R:
            STATUS_CTRL_R = make;
            key = 0;
            break;
        case KEY_ALT_L:
            STATUS_ALT_L = make;
            key = 0;
            break;
        case KEY_ALT_R:
            STATUS_ALT_R = make;
            key = 0;
            break;
        case KEY_CAPS_LOCK:
            if (make)
            {
                STATUS_CAPS_LOCK = !STATUS_CAPS_LOCK;
                keyboard_set_leds();
            }
            break;
        case KEY_NUM_LOCK:
            if (make)
            {
                STATUS_NUM_LOCK = !STATUS_NUM_LOCK;
                keyboard_set_leds();
            }
            break;
        case KEY_SCROLL_LOCK:
            if (make)
            {
                STATUS_SCROLL_LOCK = !STATUS_SCROLL_LOCK;
                keyboard_set_leds();
            }
            break;
        default:
            break;
        }

        int pad = 0;
        if ((key >= KEY_PAD_SLASH) && (key <= KEY_PAD_9))
        {
            pad = 1;
            switch (key)
            {
            case KEY_PAD_SLASH:
                key = '/';
                break;
            case KEY_PAD_STAR:
                key = '*';
                break;
            case KEY_PAD_MINUS:
                key = '-';
                break;
            case KEY_PAD_PLUS:
                key = '+';
                break;
            case KEY_PAD_ENTER:
                key = KEY_ENTER;
                break;
            default:
                if (STATUS_NUM_LOCK && (key >= KEY_PAD_0) && (key <= KEY_PAD_9))
                {
                    key = key - KEY_PAD_0 + '0';
                }
                else if (STATUS_NUM_LOCK && (key == KEY_PAD_DOT))
                {
                    key = '.';
                }
                else
                {
                    switch (key)
                    {
                    case KEY_PAD_HOME:
                        key = KEY_HOME;
                        break;
                    case KEY_PAD_END:
                        key = KEY_END;
                        break;
                    case KEY_PAD_PAGEUP:
                        key = KEY_PAGEUP;
                        break;
                    case KEY_PAD_PAGEDOWN:
                        key = KEY_PAGEDOWN;
                        break;
                    case KEY_PAD_INS:
                        key = KEY_INSERT;
                        break;
                    case KEY_PAD_UP:
                        key = KEY_UP;
                        break;
                    case KEY_PAD_DOWN:
                        key = KEY_DOWN;
                        break;
                    case KEY_PAD_LEFT:
                        key = KEY_LEFT;
                        break;
                    case KEY_PAD_RIGHT:
                        key = KEY_RIGHT;
                        break;
                    case KEY_PAD_DOT:
                        key = KEY_DELETE;
                        break;
                    default:
                        break;
                    }
                }
                break;
            }
        }
        if (make)
        {
            key |= STATUS_SHIFT_L ? FLAG_SHIFT_L : 0;
            key |= STATUS_SHIFT_R ? FLAG_SHIFT_R : 0;
            key |= STATUS_CTRL_L ? FLAG_CTRL_L : 0;
            key |= STATUS_CTRL_R ? FLAG_CTRL_R : 0;
            key |= STATUS_ALT_L ? FLAG_ALT_L : 0;
            key |= STATUS_ALT_R ? FLAG_ALT_R : 0;
            key |= pad ? FLAG_PAD : 0;
            return key;
        }
        return NULL;
    }
}

void read_keyboard(callback handler)
{
    if (keyboard.queue.count <= 0)
        return;

    int key = read_key();
    if (key == NULL)
        return;
    handler(key);
}

static void keyboard_wait()
{
    u8 var;

    do
    {
        var = in_byte(KEYBOARD_STATUS_PORT);
    } while (var & 0x02);
}

static void keyboard_ack()
{
    u8 var;
    do
    {
        var = in_byte(KEYBOARD_DATA_PORT);
    } while (var = !KEYBOARD_ACK);
}

static void keyboard_set_leds()
{
    u8 leds = (STATUS_CAPS_LOCK << 2) | (STATUS_NUM_LOCK << 1) | STATUS_SCROLL_LOCK;

    keyboard_wait();
    out_byte(KEYBOARD_DATA_PORT, KEYBOARD_LED_CODE);
    keyboard_ack();

    keyboard_wait();
    out_byte(KEYBOARD_DATA_PORT, leds);
    keyboard_ack();
}
