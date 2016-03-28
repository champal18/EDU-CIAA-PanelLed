/*
 * lcd.c
 *
 *  Created on: 30/07/2013
 *      Author: pc2
 */

// **************************************************************************
// Basic Character LCD functions
// By Fábio Pereira
// 01/15/08
// **************************************************************************

#include "lcd.h"
#include "ciaaPOSIX_stdio.h"  // <= device handler header
#include "ciaaPOSIX_string.h" // <= string header

union ubyte
{
	char _byte;
	struct
	{
		unsigned char b0 : 1;
		unsigned char b1 : 1;
		unsigned char b2 : 1;
		unsigned char b3 : 1;
		unsigned char b4 : 1;
		unsigned char b5 : 1;
		unsigned char b6 : 1;
		unsigned char b7 : 1;
	} bit;
};

//static int32_t fd_lcd;
extern int32_t fd_out;
extern uint16_t outputs;

//static uint16_t outputs_lcd;
static char lcd_mode;	


//********************** 	MIS FUNCIONES	********************************

void update_LCD(uint8_t *cadena, char numLinea, uint8_t fin)
{
	LCD_pos_xy(0,numLinea);
	LCD_write_string(cadena,fin);
}


//********************** FIN MIS FUNCIONES *********************************

//**************************************************************************
//* A simple delay function (used by LCD functions)
//**************************************************************************
//* Calling arguments
//* unsigned char time: aproximate delay time in microseconds
//**************************************************************************
void LCD_delay_ms (int time)
{
	unsigned int temp;
	for(;time;time--) for(temp=(BUS_CLOCK/23);temp;temp--);
}

//**************************************************************************
//* Send a nibble to the LCD
//**************************************************************************
//* Calling arguments
//* char data : data to be sent to the display
//**************************************************************************
void LCD_send_nibble(char data)
{
	union ubyte my_union;
	my_union._byte = data;

	//ciaaPOSIX_read(fd_lcd, &outputs_lcd, 2);

	if(my_union.bit.b0 == 0)
		outputs = outputs & 0xFBFF; 	// xxxx x0xx xxxx xxxx
	else
		outputs = outputs | 0x0400;	// xxxx x1xx xxxx xxxx
	if(my_union.bit.b1 == 0)
		outputs = outputs & 0xF7FF;	// xxxx 0xxx xxxx xxxx
	else
		outputs = outputs | 0x0800;	// xxxx 1xxx xxxx xxxx
	if(my_union.bit.b2 == 0)
		outputs = outputs & 0xEFFF;	// xxx0 xxxx xxxx xxxx
	else
		outputs = outputs | 0x1000;	// xxx1 xxxx xxxx xxxx
	if(my_union.bit.b3 == 0)
		outputs = outputs & 0xDFFF;	// xx0x xxxx xxxx xxxx
	else
		outputs = outputs | 0x2000;	// xx1x xxxx xxxx xxxx

	ciaaPOSIX_write(fd_out, &outputs, 2);

	// pulse the LCD enable line -> Mando un 1 por el bit 15

	outputs= outputs | 0x8000; 	// 1xxx xxxx xxxx xxxx
	ciaaPOSIX_write(fd_out, &outputs, 2);
	for (data=533; data; data--);	//0.06ms

	outputs = outputs & 0x7FFF;	// 0111 1111 1111 1111 Mando un 0 por el bit 15
	ciaaPOSIX_write(fd_out, &outputs, 2);
}

//**************************************************************************
//* Write a byte into the LCD
//**************************************************************************
//* Calling arguments:
//* char address : 0 for instructions, 1 for data
//* char data : command or data to be written
//**************************************************************************
void LCD_send_byte(char address, uint8_t data)
{
	if(address==0)
	{
		// Mando un 0 por el bit 14 (RS)
		//ciaaPOSIX_read(fd_lcd, &outputs_lcd, 2);
		outputs= outputs & 0xBFFF;	// x0xx xxxx xxxx xxxx
		ciaaPOSIX_write(fd_out, &outputs, 2);	// config the R/S line
	}
	else
	{
		// Mando un 1 por el bit 14 (RS)
		//ciaaPOSIX_read(fd_lcd, &outputs_lcd, 2);
		outputs= outputs | 0x4000;	// x1xx xxxx xxxx xxxx
		ciaaPOSIX_write(fd_out, &outputs, 2);
	}

	//ciaaPOSIX_read(fd_lcd, &outputs_lcd, 2);
	// Bit Enable = 0
	outputs = outputs & 0x7FFF;	// 0111 1111 1111 1111 Mando un 0 por el bit 15
	ciaaPOSIX_write(fd_out, &outputs, 2);

	LCD_send_nibble(data >> 4);     // send the higher nibble
	LCD_send_nibble(data & 0x0f);   // send the lower nibble
	LCD_delay_ms(3);				//for (temp=1000; temp; temp--);
}

//**************************************************************************
//* LCD initialization
//**************************************************************************
//* Calling arguments:
//* char mode1 : display mode (number of lines and character size)
//* char mode2 : display mode (cursor and display state)
//**************************************************************************
void LCD_init(char mode1, char mode2)
{
	char aux;

	// Set the LCD data pins to zero
	// Set the LCD data pins to zero
	outputs = outputs & 0x03FF;	// 0000 0011 1111 1111
	ciaaPOSIX_write(fd_out, &outputs, 2);

	LCD_delay_ms(15);
	// LCD 4-bit mode initialization sequence
	// send three times 0x03 and then 0x02 to finish configuring the LCD
	for(aux=0;aux<3;++aux)
	{		
	  LCD_send_nibble(3);
	  LCD_delay_ms(5);
	}
	LCD_send_nibble(2);
	// Now send the LCD configuration data
	LCD_send_byte(0,0x20 | mode1);
	LCD_send_byte(0,0x08 | mode2);
	lcd_mode = 0x08 | mode2;
	
	LCD_send_byte(0,1);
	LCD_delay_ms(5);
	LCD_send_byte(0,6); //entry mode set I/D=1 S=0
}

//**************************************************************************
//* LCD cursor position set
//**************************************************************************
//* Calling arguments:
//* char x : column (starting by 0)
//* char y : line (0 or 1)
//**************************************************************************
void LCD_pos_xy(char x, char y)
{
  char address;
  if (y)
	  address = LCD_SEC_LINE;
  else address = 0;
  address += x;
  LCD_send_byte(0,0x80|address);
}

//**************************************************************************
//* Write a character on the display
//**************************************************************************
//* Calling arguments:
//* char c : character to be written
//**************************************************************************
//* Notes :
//* \f clear the display
//* \n and \r return the cursor to line 1 column 0
//**************************************************************************
void LCD_write_char(uint8_t c)
{
  switch (c)
	{
	  case '\f' :	
	    LCD_send_byte(0,1);
	    LCD_delay_ms(5);
	    break;
	  case '\n' :
		  break;
	  case '\r' :	
	    LCD_pos_xy(0,1);
	    break;
	  default:
	    LCD_send_byte(1,c);
   }
}

//**************************************************************************
//* Write a string on the display
//**************************************************************************
//* Calling arguments:
//* char *c : pointer to the string
//**************************************************************************
void LCD_write_string (uint8_t *c, uint8_t fin)
{
	while ((*c)&&(fin))
	{
	  LCD_write_char(*c);
	  c++;
	  fin--;
	}
}

//**************************************************************************
//* Turn the display on
//**************************************************************************
void LCD_display_on(void)
{
	lcd_mode |= 4;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn the display off
//**************************************************************************
void LCD_display_off(void)
{
	lcd_mode &= 0b11111011;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn the cursor on
//**************************************************************************
void LCD_cursor_on(void)
{
  lcd_mode |= 2;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn the cursor off
//**************************************************************************
void LCD_cursor_off(void)
{
	lcd_mode &= 0b11111101;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn on the cursor blink function
//**************************************************************************
void LCD_cursor_blink_on(void)
{
	lcd_mode |= 1;
	LCD_send_byte (0,lcd_mode);
}

//**************************************************************************
//* Turn off the cursor blink function
//**************************************************************************
void LCD_cursor_blink_off(void)
{
	lcd_mode &= 0b11111110;
	LCD_send_byte (0,lcd_mode);
}
