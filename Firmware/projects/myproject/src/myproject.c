/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*==================[inclusions]=============================================*/

#include "myproject.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

uint8_t flag_cartel = 0; 	//usamos para mostrar el cartel solo 1 vez por etapa

/*==================[external data definition]===============================*/

int32_t fd_out;
int32_t fd_in;
uint16_t outputs;
State state = VELOCIDAD;

/*==================[internal functions definition]==========================*/

// Inicializacion del SPP1
void Init_SPP1(void)
{
	Chip_SSP_Init(LPC_SSP1);
	Chip_SSP_Enable(LPC_SSP1);

	// Apertura de puertos para SPI
	// MD_PUP: Enable pull-up resistor at pad
	Chip_SCU_PinMux(1, 4, MD_PUP, FUNC5);	// P1_4 SPI_MOSI
	Chip_SCU_PinMux(1, 20, MD_PUP, FUNC1);	// P1_20 TXD1 --> SSP1 SSEL
	Chip_SCU_PinMux(15, 4, MD_PUP, FUNC0);	// PF_4 SPI_SCK
}	// end of Init_SPP1

void maquina_estados(void)
{
	switch(state)
	{
		case VELOCIDAD:
		{
			if(!flag_cartel)
			{
				update_LCD("Selec Velocidad ",0,15);
				flag_cartel = 1;
			}
			botonera_CIAA();
			inicializar_sistema();
			break;
		}
		case INTENSIDAD:
		{
			if(!flag_cartel)
			{
				update_LCD("Selec Intensidad",0,16);
				flag_cartel = 1;
			}
			botonera_CIAA();
			inicializar_sistema();
			break;
		}
		case MODO:
		{
			if(!flag_cartel)
			{
				update_LCD("Selec Modo      ",0,16);
				flag_cartel = 1;
			}
			botonera_CIAA();
			inicializar_sistema();
			break;
		}
		case ACTIVO:
		{
			if(!flag_cartel)
			{
				update_LCD("--CIAA Message--",0,16);
				flag_cartel = 1;
			}
			obtener_boton();					//toma el valor del boton seleccionado
			obtener_funcionalidad();			//logica del teclado
			break;
		}
	}
}


/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */

int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */
   StartOS(AppMode1);

   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

/** \brief Error Hook function
 *
 * This fucntion is called from the os if an os interface (API) returns an
 * error. Is for debugging proposes. If called this function triggers a
 * ShutdownOs which ends in a while(1).
 *
 * The values:
 *    OSErrorGetServiceId
 *    OSErrorGetParam1
 *    OSErrorGetParam2
 *    OSErrorGetParam3
 *    OSErrorGetRet
 *
 * will provide you the interface, the input parameters and the returned value.
 * For more details see the OSEK specification:
 * http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 *
 */
void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

/** \brief Initial task
 *
 * This task is started automatically in the application mode 1.
 */
TASK(InitTask)
{
   /* init CIAA kernel and devices */
   ciaak_start();

   /* print message (only on x86) */
   ciaaPOSIX_printf("Init Task...\n");

   // Init para comunicacion SPI por SPP1
   Init_SPP1();

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);	// ciaaPOSIX_O_WRONLY
   fd_in = ciaaPOSIX_open("/dev/dio/in/0", ciaaPOSIX_O_RDONLY);

   // Init para display 2x16
	outputs = 0x00;
	LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_ON|CURSOR_BLINK);

   /*
    * Activate Periodic Task:
    * - for the fist time after 350 ticks (350 ms)
    * - and then every 50 ticks (100 ms)
    */
   SetRelAlarm(ActivatePeriodicTask, 350, 100);
   /* terminate task */
   TerminateTask();
}


TASK(SwitchesTask)
{
	while(1)
		msjScroll();

   /* terminate task */
   TerminateTask();
}

TASK(PeriodicTask)
{
	maquina_estados();
	TerminateTask();
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/