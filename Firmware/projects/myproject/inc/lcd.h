/*
 * lcd.h
 *
 *  Created on: 11/12/2015
 *      Author: Juan Pablo
 */

#ifndef LCD_H_
#define LCD_H_

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stdio.h"  // <= device handler header
#include "ciaaPOSIX_string.h" // <= string header

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

#define BUS_CLOCK 204000
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

/*==================[internal functions definition]==========================*/

/*
** ===================================================================
**     Metodo : LCD_send_nibble()
**
**     Descripcion :
**     		Envia un dato de 4 bits a traves de los puertos GPIO junto a un pulso que habilita la linea.
**     Parametros  :
**     		data: dato a enviar al display.
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_send_nibble(char data);

/*
** ===================================================================
**     Metodo : LCD_pos_xy()
**
**     Descripcion :
**     		Posicionar el cursor en una posicion determinada.
**     Parametros  :
**     		x: posicion en el eje "x" o "columna". 	(0-15)
**			y: posicion en el eje "y" o "fila".		(0-1)
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_pos_xy(char x, char y);

/*
** ===================================================================
**     Metodo : LCD_write_char()
**
**     Descripcion :
**     		Escribe un caracter en el display.
**     Parametros  :
**     		c: caracter enviado.
**     Retorna     :
**     		Nada
** ===================================================================
*/

void LCD_write_char(uint8_t c);

/*
** ===================================================================
**     Metodo : LCD_write_string()
**
**     Descripcion :
**     		Escribir una cadena de caracteres en el display.
**     Parametros  :
**     		c: cadena a mostrar.
**			fin: posicion donde termina la cadena.
**     Retorna     :
**     		Nada
** ===================================================================
*/

void LCD_write_string (uint8_t *c, uint8_t fin);

/*
** ===================================================================
**     Metodo : LCD_display_on()
**
**     Descripcion :
**     		Enciende el display.
**     Parametros  :
**			Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_display_on(void);

/*
** ===================================================================
**     Metodo : LCD_display_off()
**
**     Descripcion :
**     		Apaga el display.
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/
//**************************************************************************
//* Turn the display off
//**************************************************************************
void LCD_display_off(void);

/*
** ===================================================================
**     Metodo : LCD_cursor_on()
**
**     Descripcion :
**     		Muestra el cursor en el display.
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_cursor_on(void);

/*
** ===================================================================
**     Metodo : LCD_cursor_off()
**
**     Descripcion :
**     		Oculta el cursor en el display.
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_cursor_off(void);

/*
** ===================================================================
**     Metodo : LCD_cursor_blink_on()
**
**     Descripcion :
**     		Activa la funcion de parpadeo del cursor
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_cursor_blink_on(void);

/*
** ===================================================================
**     Metodo : LCD_cursor_blink_off()
**
**     Descripcion :
**     		Desactiva el parpadeo del cursor.
**     Parametros  :
**    		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_cursor_blink_off(void);

/*==================[external functions declaration]=========================*/

/*
** ===================================================================
**     Metodo : LCD_delay_ms()
**
**     Descripcion :
**     		Delay producido por software.
**     Parametros  :
**     		time: tiempo de duracion del delay.
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_delay_ms (int time);

/*
** ===================================================================
**     Metodo : LCD_init()
**
**     Descripcion :
**     		Inicializacion del display.
**     Parametros  :
**     		mode1: numero de lineas y tamaño de caracteres.
**			mode2: estado del cursor y display.
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_init(char mode1, char mode2);

/*
** ===================================================================
**     Metodo : update_LCD()
**
**     Descripcion :
**     		Envia al display la cadena ingresada.
**     Parametros  :
**     		cadena: puntero a la cadena a enviar.
**			numLinea: numero de linea en la cual escribir.
**			fin: numero de caracteres a enviar.
**     Retorna     :
**     		Nada
** ===================================================================
*/
void update_LCD(uint8_t *, char, uint8_t);

/*
** ===================================================================
**     Metodo : LCD_send_byte()
**
**     Descripcion :
**     		Enviar un byte al display.
**     Parametros  :
**     		address: 0 para instruccion, 1 para dato.
			data: comando o dato a enviar.
**     Retorna     :
**     		Nada
** ===================================================================
*/
void LCD_send_byte(char address, uint8_t data);


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* LCD_H_ */
