/*
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

/*==================[inclusions]=============================================*/

#include "ciaaPOSIX_stdint.h"
#include "chip.h"
#include "ssp_18xx_43xx.h"
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[internal functions definition]==========================*/

/*
** ===================================================================
**     Metodo : clearDisplay()
**
**     Descripcion :
**          Limpia el panel.
**     Parametros  :
**          Nada
**     Retorna     :
**          Nada
** ===================================================================
*/

    void clearDisplay();

/*
** ===================================================================
**     Metodo : setRow()
**
**     Descripcion :
**          Selecciona y escribe en una columna.
**     Parametros  :
**          row: numero de columna.
**          value: dato a escribir.
**     Retorna     :
**          Nada
** ===================================================================
*/

    void setRow(int row, uint8_t value);

/*
** ===================================================================
**     Metodo : printChar()
**
**     Descripcion :
**          Imprime un caracter, enviando su "mapeo" correspondiente a los caracteres predefinidos.
**     Parametros  :
**          pos: columna del panel donde debe comenzar el caracter.
**          c: caracter a mostrar.
**     Retorna     :
**          Nada
** ===================================================================
*/

    void printChar(int pos, char c);

/*
** ===================================================================
**     Metodo : printStringScroll()
**
**     Descripcion :
**          Muestra el mensaje en forma de "Scroll".
**     Parametros  :
**          pos: columna del panel donde debe comenzar a mostrarse el mensaje.
**          string: cadena a mostrar.
**          tDelay: "velocidad" en que se muestra el cartel.
**          sentido: si se muestra hacia derecha o izquierda.
**     Retorna     :
**          Nada
** ===================================================================
*/

    void printStringScroll(int pos, char string[], int tDelay, char sentido);

/*
** ===================================================================
**     Metodo : printString()
**
**     Descripcion :
**          Enviar una cadena de caracteres al panel.
**     Parametros  :
**          pos: columna del panel donde debe comenzar la cadena.
**          string: cadena de caracteres.
**     Retorna     :
**          Nada
** ===================================================================
*/

    void printString(int pos, char string[]);

/*
** ===================================================================
**     Metodo : sendByte()
**
**     Descripcion :
**          Envia un byte a traves del protocolo SPI.
**     Parametros  :
**          reg: registro de direccionamiento.
**          data: dato a enviar.
**     Retorna     :
**          Nada
** ===================================================================
*/

    void sendByte(uint8_t reg, uint8_t data);


/*==================[external functions declaration]=========================*/

/*
** ===================================================================
**     Metodo : Init_MAX7219()
**
**     Descripcion :
**          Inicializacion del panel LED.
**     Parametros  :
**          Nada
**     Retorna     :
**          Nada
** ===================================================================
*/

    void Init_MAX7219();

/*
** ===================================================================
**     Metodo : msjScroll()
**
**     Descripcion :
**          De acuerdo al "modo" seleccionado muestra el mensaje por el panel Led.
**     Parametros  :
**          Nada
**     Retorna     :
**          Nada
** ===================================================================
*/

	void msjScroll();
