/*
 * lcd.c
 *
 *  Created on: 11/12/2015
 *      Author: Juan Pablo
 */

/*==================[inclusions]=============================================*/

#include "lcd.h"

/*==================[internal data declaration]==============================*/

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
static char lcd_mode;

/*==================[external data definition]===============================*/

extern int32_t fd_out;
extern uint16_t outputs;

/*==================[internal functions definition]==========================*/

void LCD_send_nibble(char data)
{
	union ubyte my_union;
	my_union._byte = data;

	if(my_union.bit.b0 == 0)
		outputs = outputs & 0xFBFF; // xxxx x0xx xxxx xxxx
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

	ciaaPOSIX_write(fd_out, &outputs, 2);	// Se envia el dato por la salida GPIO

	// pulse the LCD enable line
	outputs= outputs | 0x8000; 	// 1xxx xxxx xxxx xxxx -> Envio un 1 por el bit 15
	ciaaPOSIX_write(fd_out, &outputs, 2);

	for (data=533; data; data--);	// Delay

	outputs = outputs & 0x7FFF;	// 0xxx xxxx xxxx xxxx -> Envio un 0 por el bit 15
	ciaaPOSIX_write(fd_out, &outputs, 2);
}

void LCD_pos_xy(char x, char y)
{
  char address;
  if (y)
	  address = LCD_SEC_LINE;
  else address = 0;
  address += x;
  LCD_send_byte(0,0x80|address);
}

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

void LCD_write_string (uint8_t *c, uint8_t fin)
{
	while ((*c) && (fin > 0))
	{
	  LCD_write_char(*c);
	  c++;
	  fin--;
	}
}

void LCD_display_on(void)
{
	lcd_mode |= 4;
	LCD_send_byte (0,lcd_mode);
}

void LCD_display_off(void)
{
	lcd_mode &= 0b11111011;
	LCD_send_byte (0,lcd_mode);
}

void LCD_cursor_on(void)
{
  lcd_mode |= 2;
	LCD_send_byte (0,lcd_mode);
}

void LCD_cursor_off(void)
{
	lcd_mode &= 0b11111101;
	LCD_send_byte (0,lcd_mode);
}

void LCD_cursor_blink_on(void)
{
	lcd_mode |= 1;
	LCD_send_byte (0,lcd_mode);
}

void LCD_cursor_blink_off(void)
{
	lcd_mode &= 0b11111110;
	LCD_send_byte (0,lcd_mode);
}



/*==================[external functions definition]==========================*/

void update_LCD(uint8_t *cadena, char numLinea, uint8_t fin)
{
	LCD_pos_xy(0,numLinea);
	LCD_write_string(cadena,fin);
}

void LCD_delay_ms (int time)
{
	unsigned int temp;
	for(;time;time--) for(temp=(BUS_CLOCK/23);temp;temp--);
}

void LCD_send_byte(char address, uint8_t data)
{
	if(address==0)
	{
		outputs= outputs & 0xBFFF;	// x0xx xxxx xxxx xxxx -> Envio un 0 por el bit 14 (RS)
		ciaaPOSIX_write(fd_out, &outputs, 2);	// config the R/S line
	}
	else
	{
		outputs= outputs | 0x4000;	// x1xx xxxx xxxx xxxx -> Envio un 1 por el bit 14 (RS)
		ciaaPOSIX_write(fd_out, &outputs, 2);	// config the R/S line
	}

	outputs = outputs & 0x7FFF;		// 0111 1111 1111 1111 -> Envio un 0 por el bit 15
	ciaaPOSIX_write(fd_out, &outputs, 2);

	LCD_send_nibble(data >> 4);     // send the higher nibble
	LCD_send_nibble(data & 0x0f);   // send the lower nibble
	LCD_delay_ms(3);
}

void LCD_init(char mode1, char mode2)
{
	char aux;

	// Set the LCD data pins to zero
	outputs = outputs & 0x03FF;	// 0000 00xx xxxx xxxx
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


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/