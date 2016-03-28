/*
 * lcd.h
 *
 *  Created on: 30/07/2013
 *      Author: pc2
 */

#ifndef LCD_H_
#define LCD_H_

#include "ciaaPOSIX_stdint.h"

#define LCD_SEC_LINE    0x40    // Address of the second line of the LCD

// LCD configuration constants
#define CURSOR_ON       2
#define CURSOR_OFF      0
#define CURSOR_BLINK    1
#define CURSOR_NOBLINK  0
#define DISPLAY_ON      4
#define DISPLAY_OFF     0
#define DISPLAY_8X5     0
#define DISPLAY_10X5    4
#define _2_LINES        8
#define _1_LINE         0

//**************************************************************************
//* Prototypes
//**************************************************************************
void LCD_delay_ms (int time);
void LCD_send_nibble(char data);
void LCD_send_byte(char address, uint8_t data);
void LCD_init(char mode1, char mode2);
void LCD_pos_xy(char x, char y);
void LCD_write_char(uint8_t c);
void LCD_write_string (uint8_t *c,uint8_t);
void LCD_display_on(void);
void LCD_display_off(void);
void LCD_cursor_on(void);
void LCD_cursor_off(void);
void LCD_cursor_blink_on(void);
void LCD_cursor_blink_off(void);
void update_LCD(uint8_t *, char, uint8_t);

#define BUS_CLOCK 204000

#endif /* LCD_H_ */
