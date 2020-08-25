#include <onix/keyboard.h>
#include <onix/interrupt.h>
#include <onix/stdio.h>
#include <onix/io.h>
#include <onix/tty.h>

KeyboardInput kinput;

u32 keymap[NR_SCAN_CODES * MAP_COLS] = {

    /* scan-code            !Shift      Shift    E0 XX  */
    /* ==================================================================== */
    /* 0x00 - none      */ 0, 0, 0,
    /* 0x01 - ESC       */ ESC, ESC, 0,
    /* 0x02 - '1'       */ '1', '!', 0,
    /* 0x03 - '2'       */ '2', '@', 0,
    /* 0x04 - '3'       */ '3', '#', 0,
    /* 0x05 - '4'       */ '4', '$', 0,
    /* 0x06 - '5'       */ '5', '%', 0,
    /* 0x07 - '6'       */ '6', '^', 0,
    /* 0x08 - '7'       */ '7', '&', 0,
    /* 0x09 - '8'       */ '8', '*', 0,
    /* 0x0A - '9'       */ '9', '(', 0,
    /* 0x0B - '0'       */ '0', ')', 0,
    /* 0x0C - '-'       */ '-', '_', 0,
    /* 0x0D - '='       */ '=', '+', 0,
    /* 0x0E - BS        */ BACKSPACE, BACKSPACE, 0,
    /* 0x0F - TAB       */ TAB, TAB, 0,
    /* 0x10 - 'q'       */ 'q', 'Q', 0,
    /* 0x11 - 'w'       */ 'w', 'W', 0,
    /* 0x12 - 'e'       */ 'e', 'E', 0,
    /* 0x13 - 'r'       */ 'r', 'R', 0,
    /* 0x14 - 't'       */ 't', 'T', 0,
    /* 0x15 - 'y'       */ 'y', 'Y', 0,
    /* 0x16 - 'u'       */ 'u', 'U', 0,
    /* 0x17 - 'i'       */ 'i', 'I', 0,
    /* 0x18 - 'o'       */ 'o', 'O', 0,
    /* 0x19 - 'p'       */ 'p', 'P', 0,
    /* 0x1A - '['       */ '[', '{', 0,
    /* 0x1B - ']'       */ ']', '}', 0,
    /* 0x1C - CR/LF     */ ENTER, ENTER, PAD_ENTER,
    /* 0x1D - CTRL_R    */ CTRL_L, CTRL_L, CTRL_R,
    /* 0x1E - 'a'       */ 'a', 'A', 0,
    /* 0x1F - 's'       */ 's', 'S', 0,
    /* 0x20 - 'd'       */ 'd', 'D', 0,
    /* 0x21 - 'f'       */ 'f', 'F', 0,
    /* 0x22 - 'g'       */ 'g', 'G', 0,
    /* 0x23 - 'h'       */ 'h', 'H', 0,
    /* 0x24 - 'j'       */ 'j', 'J', 0,
    /* 0x25 - 'k'       */ 'k', 'K', 0,
    /* 0x26 - 'l'       */ 'l', 'L', 0,
    /* 0x27 - ';'       */ ';', ':', 0,
    /* 0x28 - '\''      */ '\'', '"', 0,
    /* 0x29 - '`'       */ '`', '~', 0,
    /* 0x2A - SHIFT_l   */ SHIFT_L, SHIFT_L, 0,
    /* 0x2B - '\'       */ '\\', '|', 0,
    /* 0x2C - 'z'       */ 'z', 'Z', 0,
    /* 0x2D - 'x'       */ 'x', 'X', 0,
    /* 0x2E - 'c'       */ 'c', 'C', 0,
    /* 0x2F - 'v'       */ 'v', 'V', 0,
    /* 0x30 - 'b'       */ 'b', 'B', 0,
    /* 0x31 - 'n'       */ 'n', 'N', 0,
    /* 0x32 - 'm'       */ 'm', 'M', 0,
    /* 0x33 - ','       */ ',', '<', 0,
    /* 0x34 - '.'       */ '.', '>', 0,
    /* 0x35 - '/'       */ '/', '?', PAD_SLASH,
    /* 0x36 - SHIFT_R   */ SHIFT_R, SHIFT_R, 0,
    /* 0x37 - '*'       */ '*', '*', 0,
    /* 0x38 - ALT       */ ALT_L, ALT_L, ALT_R,
    /* 0x39 - ' '       */ ' ', ' ', 0,
    /* 0x3A - CapsLock  */ CAPS_LOCK, CAPS_LOCK, 0,
    /* 0x3B - F1        */ F1, F1, 0,
    /* 0x3C - F2        */ F2, F2, 0,
    /* 0x3D - F3        */ F3, F3, 0,
    /* 0x3E - F4        */ F4, F4, 0,
    /* 0x3F - F5        */ F5, F5, 0,
    /* 0x40 - F6        */ F6, F6, 0,
    /* 0x41 - F7        */ F7, F7, 0,
    /* 0x42 - F8        */ F8, F8, 0,
    /* 0x43 - F9        */ F9, F9, 0,
    /* 0x44 - F10       */ F10, F10, 0,
    /* 0x45 - NumLock   */ NUM_LOCK, NUM_LOCK, 0,
    /* 0x46 - ScrLock   */ SCROLL_LOCK, SCROLL_LOCK, 0,
    /* 0x47 - Home      */ PAD_HOME, '7', HOME,
    /* 0x48 - CurUp     */ PAD_UP, '8', UP,
    /* 0x49 - PgUp      */ PAD_PAGEUP, '9', PAGEUP,
    /* 0x4A - '-'       */ PAD_MINUS, '-', 0,
    /* 0x4B - Left      */ PAD_LEFT, '4', LEFT,
    /* 0x4C - MID       */ PAD_MID, '5', 0,
    /* 0x4D - Right     */ PAD_RIGHT, '6', RIGHT,
    /* 0x4E - '+'       */ PAD_PLUS, '+', 0,
    /* 0x4F - End       */ PAD_END, '1', END,
    /* 0x50 - Down      */ PAD_DOWN, '2', DOWN,
    /* 0x51 - PgDown    */ PAD_PAGEDOWN, '3', PAGEDOWN,
    /* 0x52 - Insert    */ PAD_INS, '0', INSERT,
    /* 0x53 - Delete    */ PAD_DOT, '.', DELETE,
    /* 0x54 - Enter     */ 0, 0, 0,
    /* 0x55 - ???       */ 0, 0, 0,
    /* 0x56 - ???       */ 0, 0, 0,
    /* 0x57 - F11       */ F11, F11, 0,
    /* 0x58 - F12       */ F12, F12, 0,
    /* 0x59 - ???       */ 0, 0, 0,
    /* 0x5A - ???       */ 0, 0, 0,
    /* 0x5B - ???       */ 0, 0, GUI_L,
    /* 0x5C - ???       */ 0, 0, GUI_R,
    /* 0x5D - ???       */ 0, 0, APPS,
    /* 0x5E - ???       */ 0, 0, 0,
    /* 0x5F - ???       */ 0, 0, 0,
    /* 0x60 - ???       */ 0, 0, 0,
    /* 0x61 - ???       */ 0, 0, 0,
    /* 0x62 - ???       */ 0, 0, 0,
    /* 0x63 - ???       */ 0, 0, 0,
    /* 0x64 - ???       */ 0, 0, 0,
    /* 0x65 - ???       */ 0, 0, 0,
    /* 0x66 - ???       */ 0, 0, 0,
    /* 0x67 - ???       */ 0, 0, 0,
    /* 0x68 - ???       */ 0, 0, 0,
    /* 0x69 - ???       */ 0, 0, 0,
    /* 0x6A - ???       */ 0, 0, 0,
    /* 0x6B - ???       */ 0, 0, 0,
    /* 0x6C - ???       */ 0, 0, 0,
    /* 0x6D - ???       */ 0, 0, 0,
    /* 0x6E - ???       */ 0, 0, 0,
    /* 0x6F - ???       */ 0, 0, 0,
    /* 0x70 - ???       */ 0, 0, 0,
    /* 0x71 - ???       */ 0, 0, 0,
    /* 0x72 - ???       */ 0, 0, 0,
    /* 0x73 - ???       */ 0, 0, 0,
    /* 0x74 - ???       */ 0, 0, 0,
    /* 0x75 - ???       */ 0, 0, 0,
    /* 0x76 - ???       */ 0, 0, 0,
    /* 0x77 - ???       */ 0, 0, 0,
    /* 0x78 - ???       */ 0, 0, 0,
    /* 0x78 - ???       */ 0, 0, 0,
    /* 0x7A - ???       */ 0, 0, 0,
    /* 0x7B - ???       */ 0, 0, 0,
    /* 0x7C - ???       */ 0, 0, 0,
    /* 0x7D - ???       */ 0, 0, 0,
    /* 0x7E - ???       */ 0, 0, 0,
    /* 0x7F - ???       */ 0, 0, 0};

static int code_with_E0 = 0;
static int shift_l;     /* l shift state */
static int shift_r;     /* r shift state */
static int alt_l;       /* l alt state	 */
static int alt_r;       /* r left state	 */
static int ctrl_l;      /* l ctrl state	 */
static int ctrl_r;      /* l ctrl state	 */
static int caps_lock;   /* Caps Lock	 */
static int num_lock;    /* Num Lock	 */
static int scroll_lock; /* Scroll Lock	 */
static int column;

u8 get_input_code()
{
    while (kinput.count <= 0)
    {
    }
    io_cli();
    u8 keycode = *(kinput.tail);
    kinput.tail++;
    if (kinput.tail == kinput.buf + KB_IN_BYTES)
    {
        kinput.tail = kinput.buf;
    }
    kinput.count--;
    io_sti();
    return keycode;
}

void read_keyboard(TTY *tty)
{
    int key = 0;
    bool make;
    if (kinput.count <= 0)
        return;

    code_with_E0 = 0;
    u8 keycode = get_input_code();

    if (keycode == 0xE1)
    {
        int i;
        u8 pausebrk_scode[] = {0xE1, 0x1D, 0x45,
                               0xE1, 0x9D, 0xC5};
        int is_pausebreak = 1;
        for (i = 1; i < 6; i++)
        {
            if (get_input_code() != pausebrk_scode[i])
            {
                is_pausebreak = 0;
                break;
            }
        }
        if (is_pausebreak)
        {
            key = PAUSEBREAK;
        }
    }
    if (keycode == 0xE0)
    {
        keycode = get_input_code();

        /* PrintScreen down */
        if (keycode == 0x2A)
        {
            if (get_input_code() == 0xE0)
            {
                if (get_input_code() == 0x37)
                {
                    key = PRINTSCREEN;
                    make = 1;
                }
            }
        }
        /* PrintScreen up */
        if (keycode == 0xB7)
        {
            if (get_input_code() == 0xE0)
            {
                if (get_input_code() == 0xAA)
                {
                    key = PRINTSCREEN;
                    make = 0;
                }
            }
        }
        if (key == 0)
        {
            code_with_E0 = 1;
        }
    }
    if ((key != PAUSEBREAK) && (key != PRINTSCREEN))
    {
        make = keycode & FLAG_BREAK ? false : true;
        int index = (keycode & 0x7F) * MAP_COLS;
        u32 *keyrow = &keymap[index];

        column = 0;
        if (shift_l || shift_r)
        {
            column = 1;
        }
        if (code_with_E0)
        {
            column = 2;
            code_with_E0 = 0;
        }

        u32 key = keyrow[column];
        switch (key)
        {
        case SHIFT_L:
            shift_l = make;
            key = 0;
            break;
        case SHIFT_R:
            shift_r = make;
            key = 0;
            break;
        case CTRL_L:
            ctrl_l = make;
            key = 0;
            break;
        case CTRL_R:
            ctrl_r = make;
            key = 0;
            break;
        case ALT_L:
            alt_l = make;
            key = 0;
            break;
        case ALT_R:
            alt_r = make;
            key = 0;
            break;
        default:
            break;
        }
        if (make)
        {
            key |= shift_l ? FLAG_SHIFT_L : 0;
            key |= shift_r ? FLAG_SHIFT_R : 0;
            key |= ctrl_l ? FLAG_CTRL_L : 0;
            key |= ctrl_r ? FLAG_CTRL_R : 0;
            key |= alt_l ? FLAG_ALT_L : 0;
            key |= alt_r ? FLAG_ALT_R : 0;
            in_process(tty, key);
        }
    }
}

void keyboard_handler(int irq)
{
    // io_outb(INT_M_CTL, EOI);

    int status;
    int keycode;

    status = io_inb(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (!(status & 0x01))
        return;
    keycode = io_inb(KEYBOARD_DATA_PORT);
    if (keycode < 0)
        return;
    if (kinput.count >= KB_IN_BYTES)
        return;

    *(kinput.head) = keycode;
    kinput.head++;
    if (kinput.head == kinput.buf + KB_IN_BYTES)
    {
        kinput.head = kinput.buf;
    }
    kinput.count++;
}

void init_keyboard()
{
    kinput.count = 0;
    kinput.head = kinput.tail = kinput.buf;

    shift_l = shift_r = 0;
    alt_l = alt_r = 0;
    ctrl_l = ctrl_r = 0;

    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}