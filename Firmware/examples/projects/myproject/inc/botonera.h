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
typedef enum state{VELOCIDAD, INTENSIDAD, MODO, ACTIVO} State;
uint8_t get_charmap(uint8_t, uint8_t);
void escribir(void);
void botones(void);
void botonera_reducida(void);
void escribir_reducido(void);
#endif /* BOTONERA_H_ */
