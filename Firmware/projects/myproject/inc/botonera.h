/*
 * botonera.h
 *
 *  Created on: 30/11/2015
 *      Author: Cristian
 */

#ifndef BOTONERA_H_
#define BOTONERA_H_

#include "ciaak.h"
#include "chip.h"
#include "os.h"
#include "ciaaPOSIX_stdint.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "lcd.h"

// esta maquina de estados nos servira para realizar la incializacion por etapas.
typedef enum state{VELOCIDAD, INTENSIDAD, MODO, ACTIVO} State;

/*
** ===================================================================
**     Metodo : get_charmap()
**
**     Descripcion :
**     		Obtiene el caracter correspondiente dependiendo de las coordenadas
**     		que se ingresan.
**     Parametros  :
**     		Fila: Indica la fila en la que se posiciona el caracter
**     		deseado.
**     		Columna: Indica la fila en la que se posiciona el caracter
**     		deseado.
**     Retorna     :
**     		Retorna el caracter que se busca.
** ===================================================================
*/


uint8_t get_charmap(uint8_t fila, uint8_t columna);

/*
** ===================================================================
**     Metodo : contar_tiempo()
**
**     Descripcion :
**     		Cuenta el tiempo que trascurrio sin que se haya tocado
**     		un boton luego de haber tocado uno. Si se cumple el tiempo
**     		automaticamente confirma la tecla y pasa a la siguiente
**     		posicion.
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void contar_tiempo(void);

/*
** ===================================================================
**     Metodo : check_boton_anterior()
**
**     Descripcion :
**     		Mira si hubo un boton tocado anterior mente y si fue asi,
**     		diferencia si no fue el mismo que se preciono en ese caso.
**     		En caso de que haya sido distinto, borra el contador del
**     		boton anterior y setea al actual como tal, ademas de tambien
**     		poner el flag fist touch en 1 indicando que fue la primera
**     		vez qu se toco ese boton.
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void check_boton_anterior(void);

/*
** ===================================================================
**     Metodo : mayusculas()
**
**     Descripcion :
**			mira si esta activado el boton shift. Activa o desactiva
**			las mayusculas segun corresponda
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void mayusculas(void);

/*
** ===================================================================
**     Metodo : primera_vez()
**
**     Descripcion :
**     		Cada vez que se apreta un boton, si este fue apretado luego
**     		de una confirmacion anterior, fue el primero en tocarse al
**     		comienzo de la ejecucion del programa o anteriormente el boton
**     		que se habia tocado era otro distinto, se activa el flag
**     		de first touch para acomodar a la matriz en la posicion
**     		inicial segun corresponda (si tiene mayusculas el boton
**     		se posicionara en el primer lugar par, sino se posicionara
**     		en el primer lugar impar)
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void primera_vez(void);

/*
** ===================================================================
**     Metodo : escribir_buffer()
**
**     Descripcion :
**     		una vez que se definio la logica del boton seleccionado
**     		se procede a guardarlo en el mensaje_buffer y luego escribir
**     		dicho mensaje en el LCD.
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void escribir_buffer(void);

/*
** ===================================================================
**     Metodo : funcionalidad_numeros()
**
**     Descripcion :
**     		Diferencia si se toca un numero con 3 caracteres, con 4
**     		o los especiales (signos). Suma en la posicion adecuada
**     		y la cantidad adecuada en la matriz auxiliar.
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void funcionalidad_numeros(void);

/*
** ===================================================================
**     Metodo : funcionalidad_confirmar()
**
**     Descripcion :
**     		Confirma el caracter que se estuvo seleccionando y reinicia
**     		todas las variables de entorno para procesar el siguiente
**     		caracter
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void funcionalidad_confirmar(void);

/*
** ===================================================================
**     Metodo : funcionalidad_derecha()
**
**     Descripcion :
**     		Mueve el cursor a la derecha mientras no sea final de la
**     		cadena
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void funcionalidad_derecha(void);

/*
** ===================================================================
**     Metodo : funcionalidad_shift()
**
**     Descripcion :
**     		Activa o desactiva el boton shift. Este boton hace que
**     		se alterne entre mayusculas y minusculas y solo afecta a
**     		los botones del 2 al 9
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void funcionalidad_shift(void);

/*
** ===================================================================
**     Metodo : funcionalidad_borrar_cancelar()
**
**     Descripcion :
**     		Borra el ultimo caracter escrito o, si se esta escribiendo
**     		uno, cancela la accion
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void funcionalidad_borrar_cancelar(void);

/*
** ===================================================================
**     Metodo : funcionalidad_izquierda()
**
**     Descripcion :
**     		Mueve el cursor a la izquierda mientras no se haya llegado
**     		a la posicion 0
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/


void funcionalidad_izquierda(void);

/*
** ===================================================================
**     Metodo : funcionalidad_enter()
**
**     Descripcion :
**     		Reinicia todas las variables de entorno, carga el vector
**     		con el mensaje actual a partir del vector mensaje buffer
**     		y activa la tarea que envia el cartel al panel LED
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void funcionalidad_enter(void);

/*
** ===================================================================
**     Metodo : obtener_funcionalidad()
**
**     Descripcion :
**			Agrupa todas las funcionalidades anteriores descriptas.
**			espera 3 ciclos luego de que se presiono el boton para
**			disminuir la velocidad de procesamiento y el usuario pueda
**			utilizar la botonera correctamente
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void obtener_funcionalidad(void);

/*
** ===================================================================
**     Metodo : obtener_boton()
**
**     Descripcion :
**			Realiza un barrido por todos los botones de la matriz y
**			la CIAA y determina cual de ellos fue presionado
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void obtener_boton(void);

/*
** ===================================================================
**     Metodo : botonera_CIAA()
**
**     Descripcion :
**			Sub funcion de la anterior que solo detecta si se presiono
**			algun boton de la CIAA.
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void botonera_CIAA(void);

/*
** ===================================================================
**     Metodo : inicializar_sistema()
**
**     Descripcion :
**			contiene la logica de inicializacion dependiendo de en que
**			estado este el sistema. Los estados de inicializacion son 3
**				VELOCIDAD: el usuario elije la velocidad del cartel
**				INTENSIDAD: el usuario elije la intensidad de luz de los LEDs
**				MODO: Se elije el modo de operacion del panel LED, Hay 3 modos:
**					0. Se muestran todas las letras 1 por una.
**					1. Se muestran las letras con un scroll en 1 solo sentido
**					2. Se muestran con un scroll que va en un sentido y luego vuelve
**					   en el opuesto (ping pong)
**			luego de pasar por ellos se pasa al estado ACTIVO.
**     Parametros  :
**     		Nada
**     Retorna     :
**     		Nada
** ===================================================================
*/

void inicializar_sistema(void);

/*
uint8_t get_charmap(uint8_t fila, uint8_t columna);
void contar_tiempo(void);
void check_boton_anterior(void);
void mayusculas(void);
void primera_vez(void);
void escribir_buffer(void);
void funcionalidad_numeros(void);
void funcionalidad_confirmar(void);
void funcionalidad_derecha(void);
void funcionalidad_shift(void);
void funcionalidad_borrar_cancelar(void);
void funcionalidad_izquierda(void);
void funcionalidad_enter(void);
void obtener_funcionalidad(void);
void obtener_boton(void);
void botonera_CIAA(void);
void inicializar_sistema(void);
*/

#endif /* BOTONERA_H_ */
