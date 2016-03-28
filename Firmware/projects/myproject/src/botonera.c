/*
 * botonera.c
 *
 *  Created on: 30/11/2015
 *      Author: Cristian
 */

#include "botonera.h"
#include "LedControl.h"

const uint8_t MAX_LENGTH_MESSAGE = 17; //largo de una linea del LCD
const uint8_t MAX_TIME_WAITING = 20; //suponiendo que SwitchesTask se ejecuta cada 50 ms, necesito 60 ciclos para contar 3 seg

//etiquetas botones
const uint8_t CONFIRMAR = 10;
const uint8_t BORRAR = 11;
const uint8_t SHIFT = 12;
const uint8_t IZQUIERDA = 13;
const uint8_t DERECHA = 14;
const uint8_t ENTER = 15;

uint8_t boton_seleccionado;

extern int32_t fd_in;
extern int32_t fd_out;
extern uint16_t outputs;
uint16_t inputs;





uint8_t mensaje_buffer[17] = "                ",
		mensaje_actual[17] = "                ",
		cursor = 0,
		matrix_aux[16] = {0}, 		// 16 son la cantidad de botones que usara la botonera
		shift = 0,
		mayus = 0,
		boton_actual = 0,
		conteo_salto = 0,			//cuenta hasta un valor determinado si no se toco ninguna tecla y flag escribiendo esta activo. Si llega al limite, confirma la tecla y reinicia todos los estados
		conteo_funcionalidad = 0,	//delay para que el sistema no tenga una respuesta tan rapida que el usuario no pueda percibir
		flag_escribiendo = 0,		//indica si hay una tecla presionada y aun no se ha confirmado
		first_touch = 0,			//cada vez que se toca una tecla por primera vez despues de un tiempo de salto o despues de otra tecla distinta (Ejem: si venias tocando el 2 y tocas el 3 despues de eso), este flag se activa y habilita la seccion fist_touch que posiciona en la matriz auxiliar de conteo el contador en 1 o 2 en la posicion correspondiente
		flag_inicio = 0,			//despues del primer enter, se ejecuta la tarea que muestra la palabra en el panel LED
		flag_inicializacion = 0;	//esto es para evitar que se muestre el 0 en las opciones de calibracion.



/*
** ===================================================================
**
**     Mapa de caracteres
**
**     Descripcion :
**     		Esta matriz contendra todos los caracteres que se
**     		podran escribir en el LCD y en el panel LED. La primera
**     		posicion se pone en blanco porque no se contabiliza. Esto
**     		pasa porque se utiliza un arreglo para ir contando cuantas
**     		veces se toco un determinado boton. El software detecta
**     		que si este arreglo contiene 0 en una posicion indicara que no
**     		se toco ese boton, por lo tanto habra caracteres a partir
**     		de la posicion 1 y no de la 0 como se acostumbra en una matriz
**     		normal.
**
** ===================================================================
*/

uint8_t charmap[10][11] = {
				{' ',' ','?','!','#','$','%','&','*','(',')'},		//0
				{' ','0','1','2','3','4','5','6','7','8','9'},		//1
				{' ','a','A','b','B','c','C',' ',' ',' ',' '},		//2
				{' ','d','D','e','E','f','F',' ',' ',' ',' '},		//3
				{' ','g','G','h','H','i','I',' ',' ',' ',' '},		//4
				{' ','j','J','k','K','l','L',' ',' ',' ',' '},		//5
				{' ','m','M','n','N','o','O',' ',' ',' ',' '},		//6
				{' ','p','P','q','Q','r','R','s','S',' ',' '},		//7
				{' ','t','T','u','U','v','V',' ',' ',' ',' '},		//8
				{' ','w','W','x','X','y','Y','z','Z',' ',' '}		//9
				};


extern State state;
extern uint8_t 	speed,
				intensity,
				mode,
				flag_cartel;

uint8_t get_charmap(uint8_t fila, uint8_t columna)
{
	return (charmap[fila][columna]);
}


void contar_tiempo()
{
	if (conteo_salto < MAX_TIME_WAITING)
	  conteo_salto++;
	else                              //se cumplio el tiempo de espera
	{
	  conteo_salto = 0;
	  flag_escribiendo = 0;
	  cursor++;
	  matrix_aux[boton_actual] = 0;
	  boton_actual = 'e';         	//empty
	  boton_seleccionado = 'e';   	//empty
	}
}

void check_boton_anterior()
{
	if ((flag_escribiendo) && (boton_actual != boton_seleccionado))
	{
		matrix_aux[boton_actual] = 0; //por si cambio de boton antes de que se confirmara la letra
		first_touch = 1;
		boton_actual = boton_seleccionado;
	}
	else
	{
		// Si no entramos a este if, quiere decir que volvimos a tocar el mismo boton.
		if (!flag_escribiendo)  //llegamos aca si venimos desde un estado que no tenia nungun boton apretado
		{
		  flag_escribiendo = 1;
		  first_touch = 1;
		  boton_actual = boton_seleccionado;
		}
	}
}

void mayusculas()
{
	if (shift)
	{
	  if (!mayus)  //si no me estyo moviendo por las mayusculas me muevo 1
	  {
		matrix_aux[boton_actual]++; //paso a una posicion par
		mayus = 1;
	  }
	  else
	  {
		matrix_aux[boton_actual]--; //vuelvo a una posicion impar
		mayus = 0;
	  }
	  shift = 0;
	}
}

void primera_vez()
{
	if (first_touch)
	{
	  if(mayus)
		matrix_aux[boton_actual] = 2;
	  else
		matrix_aux[boton_actual] = 1;
	  first_touch = 0;
	}
}

void escribir_buffer()
{
	mensaje_buffer[cursor] = get_charmap(boton_actual,matrix_aux[boton_actual]);
	update_LCD(mensaje_buffer,1,cursor+1);
	conteo_salto = 0;
}

void funcionalidad_numeros()
{
	if ((boton_seleccionado >= 0) && (boton_seleccionado < 10)) //significa que estoy dentro de una tecla de escritura
	{
		check_boton_anterior();

		// BOTONES DEL 2 al 9
		if ((boton_actual > 1) && (boton_actual < 10))
		{
			mayusculas();
			//Movimiento ciclico
			if ((boton_actual == 2)|(boton_actual == 3)|(boton_actual == 4)|(boton_actual == 5)|(boton_actual == 6)|(boton_actual == 8))  //esto para los numeros 2, 3, 4, 5 y 8
			{
				if (mayus) //para los pares (mayusculas)
				{
					if (matrix_aux[boton_actual] >= 6)
						matrix_aux[boton_actual] = 2;
					else
						matrix_aux[boton_actual] += 2;
				}
				else //para los impares (minusculas)
				{
					if (matrix_aux[boton_actual] >= 5)
						matrix_aux[boton_actual] = 1;
					else
						matrix_aux[boton_actual] += 2;
				}
			}
			else   //esto para los numeros 7 y 9
			{
				if ((boton_actual == 7)|(boton_actual == 9))
				{
					//para los pares (mayusculas)
					if (mayus)
					{
						if (matrix_aux[boton_actual] >= 8)
							matrix_aux[boton_actual] = 2;
						else
							matrix_aux[boton_actual] += 2;
					}
					//para los impares (minusculas)
					else
					{
						if (matrix_aux[boton_actual] >= 7)
							matrix_aux[boton_actual] = 1;
						else
							matrix_aux[boton_actual] += 2;
					}
				}
			} // fin movimiento ciclico

			// si es la primera vez que toco ese boton tengo que posicionarme al principio, ya que la matriz tiene valor 0 hasta ese momento
			primera_vez();
		}
		else // botones 0 y 1
		{
		if (matrix_aux[boton_actual] < 10)
			matrix_aux[boton_actual]++;
		else
			matrix_aux[boton_actual] = 1;
		}
		escribir_buffer();
	}
}

void funcionalidad_confirmar()
{
	if (boton_seleccionado == CONFIRMAR)
	{
		if ((flag_escribiendo) && (cursor < MAX_LENGTH_MESSAGE-1))
		{
		conteo_salto = 0;
		flag_escribiendo = 0;
		cursor++;
		matrix_aux[boton_actual] = 0;
		boton_actual = 'e';          //empty
		boton_seleccionado = 'e';    //empty
		}
	}
}

void funcionalidad_derecha()
{
	if (boton_seleccionado == DERECHA)
	{
		//la segunda parte evita que te puedas mover a posiciones vacias sin tener letras atras
		if ((cursor < MAX_LENGTH_MESSAGE-1) && (mensaje_buffer[cursor+1] != '\0'))
		{
			cursor++;
			update_LCD(mensaje_buffer,1,cursor+1);	// Escribo sin llamar la funcion, unico caso.
		}
	}
}

void funcionalidad_shift()
{
	if (boton_seleccionado == SHIFT)
	  shift = !shift;
}

void funcionalidad_borrar_cancelar()
{
	if (boton_seleccionado == BORRAR)
	{
		if (flag_escribiendo)
		{
		  conteo_salto = 0;
		  flag_escribiendo = 0;
		  matrix_aux[boton_actual] = 0;
		  boton_actual = 'e';          //empty
		  boton_seleccionado = 'e';    //empty
		}
		else
			cursor--;
		mensaje_buffer[cursor] = ' ';  			// borrar caracter del LCD
		update_LCD(mensaje_buffer,1,cursor+1);	// Escribo sin llamar la funcion, unico caso.
	}
}

void funcionalidad_izquierda()
{
	if (boton_seleccionado == IZQUIERDA)
	{
		cursor--;
		update_LCD(mensaje_buffer,1,cursor+1);	// Escribo sin llamar la funcion, unico caso.
	}
}

void funcionalidad_enter()
{
	if (boton_seleccionado == ENTER)
	{
		conteo_salto = 0;
		flag_escribiendo = 0;
		matrix_aux[boton_actual] = 0;
		boton_actual = 'e';         	//empty
		boton_seleccionado = 'e';     	//empty

		int i;

		// cargamos el mensaje antes de enviarlo
		for(i=0;i<=cursor;i++)
			mensaje_actual[i] = mensaje_buffer[i];
		mensaje_actual[cursor+1] = '\0';

		// activando la tarea para que se comience a mostar la cadena por el panel led
		if(!flag_inicio)
		{
			ActivateTask(SwitchesTask);
			flag_inicio = 1;
		}
	}
}

void obtener_funcionalidad(void)
{
	conteo_funcionalidad++;

	if (flag_escribiendo == true)
		contar_tiempo();

	if (conteo_funcionalidad == 3)
	{
		conteo_funcionalidad = 0;

		// teclas que se deshaibilitan si el cursor esta al maximo
		if (cursor < MAX_LENGTH_MESSAGE-1)
		{
			funcionalidad_numeros();
			funcionalidad_confirmar();
			funcionalidad_derecha();
			funcionalidad_shift();			//ESTA OPCION SOLO TIENE QUE AFECTAR DESDE EL BOTON 2 AL 9
		}

		// teclas que se deshabilitan si el cursor esta al minimo
		if (cursor > 0)
		{
			funcionalidad_borrar_cancelar();
			funcionalidad_izquierda();
		}

		funcionalidad_enter();
		boton_seleccionado = 'e'; //limpio el boton para que no se vuelva a ingresar sin antes haber presionado
	}
}

void botonera_CIAA()
{

	/*
	 * BOTONERA DE LA CIAA
	 *
	 * De izquierda a derecha
	 *
	 * Boton 1: SHIFT
	 * Boton 2: IZQUIERDA
	 * Boton 3: DERECHA
	 * Boton 4: ENTER
	 */

	ciaaPOSIX_read(fd_in, &inputs, 1);

	if((inputs&0x01) == 0)
		boton_seleccionado = SHIFT;

	if((inputs&0x02) == 0)
		boton_seleccionado = IZQUIERDA;

	if((inputs&0x04) == 0)
		boton_seleccionado = DERECHA;

	if((inputs&0x08) == 0)
		boton_seleccionado = ENTER;
}

void obtener_boton()
{
	uint16_t i;
	uint16_t inputs = 0x00;

	botonera_CIAA();

	//TECLADO EXTERNO

	/*
	* SALIDAS (outputs)
	*
	*Utilizamos el registro outputs configurado para 16 bits (2 bytes)
	*
	* Bits: 7 al 10 -> xxxx xx11 11xx xxxx
	*
	* Las mantenemos en alto a todas menos a la que queremos testear.
	* Utilizamos pull-up, lo que quiere decir que esperamos un 0 al apretar un boton
	*
	*
	*
	* ENTRADAS (inputs)
	*
	* Utilizamos el registro inputs configurado para 8 bits (1 byte)
	*
	* Bits: 5 al 7 -> x111 xxxx
	*
	* preguntamos por cual de todos estara en 0
	* el resto estara en 1
	*
	*/

	//FILA 1
	outputs = outputs & 0xFFBF; 				//xxxx xx11 10xx xxxx
	outputs = outputs | 0x0380;
	ciaaPOSIX_write(fd_out, &outputs, 2);
	ciaaPOSIX_read(fd_in, &inputs, 1);

	if((inputs&0x10)==0)  		//COLUMNA 1
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x10)==0)
		   boton_seleccionado = 1;
	}


	if((inputs&0x20)==0)		//COLUMNA 2
	{
	   for(i = 0; i<0xFFF; i++);	//anti rebote
	   if((inputs&0x20)==0)			//confirme boton
		   boton_seleccionado = 2;
	}

	if((inputs&0x40)==0)		//COLUMNA 3
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x40)==0)
		   boton_seleccionado = 3;
	}


	//FILA 2
	outputs = outputs & 0xFF7F;				//xxxx xx11 01xx xxxx
	outputs = outputs | 0x0340;
	ciaaPOSIX_write(fd_out, &outputs, 2);
	ciaaPOSIX_read(fd_in, &inputs, 1);

	if((inputs&0x10)==0)  		//COLUMNA 1
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x10)==0)
		   boton_seleccionado = 4;
	}

	if((inputs&0x20)==0)		//COLUMNA 2
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x20)==0)
		   boton_seleccionado = 5;
	}

	if((inputs&0x40)==0)		//COLUMNA 3
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x40)==0)
		   boton_seleccionado = 6;
	}

	//FILA 3
	outputs = outputs & 0xFEFF;				//xxxx xx10 11xx xxxx
	outputs = outputs | 0x02c0;
	ciaaPOSIX_write(fd_out, &outputs, 2);
	ciaaPOSIX_read(fd_in, &inputs, 1);

	if((inputs&0x10)==0)  		//COLUMNA 1
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x10)==0)
		   boton_seleccionado = 7;
	}

	if((inputs&0x20)==0)		//COLUMNA 2
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x20)==0)
		   boton_seleccionado = 8;
	}

	if((inputs&0x40)==0)		//COLUMNA 3
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x40)==0)
		   boton_seleccionado = 9;
	}

	//FILA 4
	outputs = outputs & 0xFDFF;				//xxxx xx01 11xx xxxx
	outputs = outputs | 0x01c0;
	ciaaPOSIX_write(fd_out, &outputs, 2);
	ciaaPOSIX_read(fd_in, &inputs, 1);

	if((inputs&0x10)==0)		//COLUMNA 1
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x10)==0)
		   boton_seleccionado = BORRAR;
	}

	if((inputs&0x20)==0)		//COLUMNA 2
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x20)==0)
		   boton_seleccionado = 0;
	}

	if((inputs&0x40)==0)		//COLUMNA 3
	{
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x40)==0)
		   boton_seleccionado = CONFIRMAR;
	}

}

void inicializar_sistema()
{
	static uint8_t valor = 0;
	conteo_funcionalidad++;
	if(flag_inicializacion == 0)
	{
		matrix_aux[1] = 1;
		flag_inicializacion = 1;
	}
	if (conteo_funcionalidad == 3)
	{
		conteo_funcionalidad = 0;
		//usamos el boton shift de forma temporal como contador para que el usuario seleccione su opcion
		if (boton_seleccionado == SHIFT)
		{
			if (state == MODO)
			{
				if (matrix_aux[1] > 2)
					matrix_aux[1] = 0;
				switch(++matrix_aux[1])
				{
					case 1:
					{
						char palabra[] = "DELETREO        ";
						int i;
						for(i=0;i<16;i++)
							mensaje_buffer[i] = palabra[i];
						break;
					}
					case 2:
					{
						char palabra[] = "CIRCULAR        ";
						int i;
						for(i=0;i<16;i++)
							mensaje_buffer[i] = palabra[i];
						break;
					}
					case 3:
					{
						char palabra[] = "PING - PONG     ";
						int i;
						for(i=0;i<16;i++)
							mensaje_buffer[i] = palabra[i];
						break;
					}
				}
				valor = matrix_aux[1];
				update_LCD(mensaje_buffer,1,16);
			}
			else
			{
				if ((valor > 4) && (matrix_aux[1] > 4))
					matrix_aux[1] = 1;
				valor = matrix_aux[1];
				mensaje_buffer[0] = get_charmap(1,++matrix_aux[1]);
				update_LCD(mensaje_buffer,1,1);
			}
		}
		if (boton_seleccionado == ENTER)
		{
			switch (state){
				case VELOCIDAD:
				{
					speed = valor;
					matrix_aux[1] = 1;
					flag_cartel = 0;
					update_LCD(" ",1,1);
					break;
				}
				case INTENSIDAD:
				{
					intensity = valor;
					matrix_aux[1] = 0;
					flag_cartel = 0;
					update_LCD(" ",1,1);
					break;
				}

				case MODO:
				{
					mode = valor;
					matrix_aux[1] = 0;
					int i;
					for(i = 0; i<16; i++)
						mensaje_buffer[i] = ' ';
					flag_cartel = 0;
					update_LCD(" ",1,1);
				    // Init para integrado MAX7219
				    Init_MAX7219();
					break;
				}
			}
			state++;
			update_LCD("                ",1,16);
		}
		boton_seleccionado = 'e';
	}
}

