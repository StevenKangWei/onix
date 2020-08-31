#ifndef ONIX_KEYMAP_H
#define ONIX_KEYMAP_H

#include <onix/type.h>

/************************************************************************/
/*                          Macros Declaration                          */
/************************************************************************/

#define MAP_COLS 3     /* Number of columns in keymap */
#define MAP_CODES 0x80 /* Number of scan codes (rows in keymap) */

#define FLAG_BREAK 0x0080   /* Break Code   */
#define FLAG_EXT 0x0100     /* Normal function keys  */
#define FLAG_SHIFT_L 0x0200 /* Shift key   */
#define FLAG_SHIFT_R 0x0400 /* Shift key   */
#define FLAG_CTRL_L 0x0800  /* Control key   */
#define FLAG_CTRL_R 0x1000  /* Control key   */
#define FLAG_ALT_L 0x2000   /* Alternate key  */
#define FLAG_ALT_R 0x4000   /* Alternate key  */
#define FLAG_PAD 0x8000     /* keys in num pad  */

#define MASK_RAW 0x01FF /* raw key value = code passed to tty & MASK_RAW \
               the value can be found either in the keymap column 0      \
               or in the list below */

/* Special keys */
#define KEY_ESC (0x01 + FLAG_EXT)       /* Esc  */
#define KEY_TAB (0x02 + FLAG_EXT)       /* Tab  */
#define KEY_ENTER (0x03 + FLAG_EXT)     /* Enter */
#define KEY_BACKSPACE (0x04 + FLAG_EXT) /* BackSpace */

#define KEY_GUI_L (0x05 + FLAG_EXT) /* L GUI */
#define KEY_GUI_R (0x06 + FLAG_EXT) /* R GUI */
#define KEY_APPS (0x07 + FLAG_EXT)  /* APPS */

/* Shift, Ctrl, Alt */
#define KEY_SHIFT_L (0x08 + FLAG_EXT) /* L Shift */
#define KEY_SHIFT_R (0x09 + FLAG_EXT) /* R Shift */
#define KEY_CTRL_L (0x0A + FLAG_EXT)  /* L Ctrl */
#define KEY_CTRL_R (0x0B + FLAG_EXT)  /* R Ctrl */
#define KEY_ALT_L (0x0C + FLAG_EXT)   /* L Alt */
#define KEY_ALT_R (0x0D + FLAG_EXT)   /* R Alt */

/* Lock keys */
#define KEY_CAPS_LOCK (0x0E + FLAG_EXT)   /* Caps Lock */
#define KEY_NUM_LOCK (0x0F + FLAG_EXT)    /* Number Lock */
#define KEY_SCROLL_LOCK (0x10 + FLAG_EXT) /* Scroll Lock */

/* Function keys */
#define KEY_F1 (0x11 + FLAG_EXT)  /* F1  */
#define KEY_F2 (0x12 + FLAG_EXT)  /* F2  */
#define KEY_F3 (0x13 + FLAG_EXT)  /* F3  */
#define KEY_F4 (0x14 + FLAG_EXT)  /* F4  */
#define KEY_F5 (0x15 + FLAG_EXT)  /* F5  */
#define KEY_F6 (0x16 + FLAG_EXT)  /* F6  */
#define KEY_F7 (0x17 + FLAG_EXT)  /* F7  */
#define KEY_F8 (0x18 + FLAG_EXT)  /* F8  */
#define KEY_F9 (0x19 + FLAG_EXT)  /* F9  */
#define KEY_F10 (0x1A + FLAG_EXT) /* F10  */
#define KEY_F11 (0x1B + FLAG_EXT) /* F11  */
#define KEY_F12 (0x1C + FLAG_EXT) /* F12  */

/* Control Pad */
#define KEY_PRINTSCREEN (0x1D + FLAG_EXT) /* Print Screen */
#define KEY_PAUSEBREAK (0x1E + FLAG_EXT)  /* Pause/Break */
#define KEY_INSERT (0x1F + FLAG_EXT)      /* Insert */
#define KEY_DELETE (0x20 + FLAG_EXT)      /* Delete */
#define KEY_HOME (0x21 + FLAG_EXT)        /* Home  */
#define KEY_END (0x22 + FLAG_EXT)         /* End  */
#define KEY_PAGEUP (0x23 + FLAG_EXT)      /* Page Up */
#define KEY_PAGEDOWN (0x24 + FLAG_EXT)    /* Page Down */
#define KEY_UP (0x25 + FLAG_EXT)          /* Up  */
#define KEY_DOWN (0x26 + FLAG_EXT)        /* Down  */
#define KEY_LEFT (0x27 + FLAG_EXT)        /* Left  */
#define KEY_RIGHT (0x28 + FLAG_EXT)       /* Right */

/* ACPI keys */
#define KEY_POWER (0x29 + FLAG_EXT) /* Power */
#define KEY_SLEEP (0x2A + FLAG_EXT) /* Sleep */
#define KEY_WAKE (0x2B + FLAG_EXT)  /* Wake Up */

/* Num Pad */
#define KEY_PAD_SLASH (0x2C + FLAG_EXT) /* /  */
#define KEY_PAD_STAR (0x2D + FLAG_EXT)  /* *  */
#define KEY_PAD_MINUS (0x2E + FLAG_EXT) /* -  */
#define KEY_PAD_PLUS (0x2F + FLAG_EXT)  /* +  */
#define KEY_PAD_ENTER (0x30 + FLAG_EXT) /* Enter */
#define KEY_PAD_DOT (0x31 + FLAG_EXT)   /* .  */
#define KEY_PAD_0 (0x32 + FLAG_EXT)     /* 0  */
#define KEY_PAD_1 (0x33 + FLAG_EXT)     /* 1  */
#define KEY_PAD_2 (0x34 + FLAG_EXT)     /* 2  */
#define KEY_PAD_3 (0x35 + FLAG_EXT)     /* 3  */
#define KEY_PAD_4 (0x36 + FLAG_EXT)     /* 4  */
#define KEY_PAD_5 (0x37 + FLAG_EXT)     /* 5  */
#define KEY_PAD_6 (0x38 + FLAG_EXT)     /* 6  */
#define KEY_PAD_7 (0x39 + FLAG_EXT)     /* 7  */
#define KEY_PAD_8 (0x3A + FLAG_EXT)     /* 8  */
#define KEY_PAD_9 (0x3B + FLAG_EXT)     /* 9  */
#define KEY_PAD_UP KEY_PAD_8                /* Up  */
#define KEY_PAD_DOWN KEY_PAD_2              /* Down  */
#define KEY_PAD_LEFT KEY_PAD_4              /* Left  */
#define KEY_PAD_RIGHT KEY_PAD_6             /* Right */
#define KEY_PAD_HOME KEY_PAD_7              /* Home  */
#define KEY_PAD_END KEY_PAD_1               /* End  */
#define KEY_PAD_PAGEUP KEY_PAD_9            /* Page Up */
#define KEY_PAD_PAGEDOWN KEY_PAD_3          /* Page Down */
#define KEY_PAD_INS KEY_PAD_0               /* Ins  */
#define KEY_PAD_MID KEY_PAD_5               /* Middle key */
#define KEY_PAD_DEL KEY_PAD_DOT             /* Del  */

extern u32 keymap[MAP_CODES * MAP_COLS];

#endif