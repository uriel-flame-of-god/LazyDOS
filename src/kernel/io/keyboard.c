/* keyboard.c – LazyDOS keyboard driver (ASCII Ctrl support) */
#include "keyboard.h"
#include "port.h"
#include <stdint.h>

#define PS2_STATUS 0x64
#define PS2_DATA   0x60
#define STAT_OBF   1   /* output buffer full */

/* scancode set 1 */
static const char normal[128] = {
    0,0,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s',
    'd','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v',
    'b','n','m',',','.','/',0,'*',0,' ',0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const char shifted[128] = {
    0,0,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,'A','S',
    'D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V',
    'B','N','M','<','>','?',0,'*',0,' ',0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

/* modifier state */
static struct {
    uint8_t shift : 1;
    uint8_t ctrl  : 1;
    uint8_t alt   : 1;
    uint8_t del   : 1;
} state;

/* -------------------------------------------------- */

static int data_waiting(void)
{
    return inb(PS2_STATUS) & STAT_OBF;
}

int lazy_key_available(void)
{
    return data_waiting();
}

static void update_modifiers(uint8_t sc)
{
    if (sc & 0x80) {
        sc &= 0x7F;
        switch (sc) {
            case 0x2A: case 0x36: state.shift = 0; break;
            case 0x1D:             state.ctrl  = 0; break;
            case 0x38:             state.alt   = 0; break;
            case 0x53:             state.del   = 0; break;
        }
    } else {
        switch (sc) {
            case 0x2A: case 0x36: state.shift = 1; break;
            case 0x1D:             state.ctrl  = 1; break;
            case 0x38:             state.alt   = 1; break;
            case 0x53:             state.del   = 1; break;
        }
    }
}

/* -------------------------------------------------- */

char lazy_trygetchar(void)
{
    if (!data_waiting())
        return 0;

    uint8_t sc = inb(PS2_DATA);
    update_modifiers(sc);

    /* ignore key releases */
    if (sc & 0x80)
        return 0;

    /* ASCII Ctrl mappings (minimal, explicit) */
    if (state.ctrl) {
        /* Ctrl+R */
        if (sc == 0x13)    /* R */
            return 0x12;   /* ASCII DC2 */

        /* Ctrl+X */
        if (sc == 0x2D)    /* X */
            return 0x18;   /* ASCII CAN */
    }

    if (sc >= 128)
        return 0;

    const char *tbl = state.shift ? shifted : normal;
    return tbl[sc];
}

char lazy_getchar(void)
{
    char c;
    while (!(c = lazy_trygetchar()))
        ;
    return c;
}

int lazy_is_ctrl_alt_del(void)
{
    return state.ctrl && state.alt && state.del;
}
