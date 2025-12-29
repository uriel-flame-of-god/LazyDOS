/* keyboard.h  –  LazyDOS “it-types-sometimes” driver */
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

int  lazy_key_available(void);   /* 1 = scancode waiting */
char lazy_getchar(void);         /* blocking ASCII */
char lazy_trygetchar(void);      /* 0 = none ready   */
int  lazy_is_ctrl_alt_del(void); /* 1 = reboot combo */

#endif