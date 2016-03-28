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


int flag_inicio = 0;


uint8_t mensaje_buffer[17] = "                ",
		mensaje_actual[17] = "                ",
		cursor = 0,
		matrix_aux[16] = {0}, 		// 16 son la cantidad de botones que usara la botonera
		shift = 0,
		mayus = 0,
		conteo_salto = 0,
		flag_escribiendo = 0,
		boton_actual = 0,
		first_touch = 0,
		conteo_funcionalidad = 0;

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


void escribir(void)
{
	conteo_funcionalidad++;


	if (flag_escribiendo == true)
	{
		if (conteo_salto < MAX_TIME_WAITING)
		{
		  conteo_salto++;
		}
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

	if (conteo_funcionalidad == 3){
		conteo_funcionalidad = 0;
		// BLOQUEO DE TECLAS (teclas que se desahibilitan si el cursos esta al maximo)
		if (cursor < MAX_LENGTH_MESSAGE-1)
		{
			// FUNCIONALIDAD BOTON
			if ((boton_seleccionado >= 0) && (boton_seleccionado < 10)) //significa que estoy dentro de una tecla de escritura
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

			  // BOTONES DEL 2 al 9
			  if ((boton_actual > 1) && (boton_actual < 10))
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

				//Movimiento ciclico
				if ((boton_actual == 2)|(boton_actual == 3)|(boton_actual == 4)|(boton_actual == 5)|(boton_actual == 6)|(boton_actual == 8))  //esto para los numeros 2, 3, 4, 5 y 8
				{
					if (mayus) //para los pares (mayusculas)
					{
					  if (matrix_aux[boton_actual] >= 6)
					  {
						matrix_aux[boton_actual] = 2;
					  }
					  else
					  {
						matrix_aux[boton_actual] += 2;
					  }

					}
					else //para los impares (minusculas)
					{
					  if (matrix_aux[boton_actual] >= 5)
					  {
						matrix_aux[boton_actual] = 1;
					  }
					  else
					  {
						matrix_aux[boton_actual] += 2;
					  }
					}
				}
				else   //esto para los numeros 7 y 9
				{
					if ((boton_actual == 7)|(boton_actual == 9))
					{
					  if (mayus) //para los pares (mayusculas)
					  {
						if (matrix_aux[boton_actual] >= 8)
						{
						  matrix_aux[boton_actual] = 2;
						}

						else
						{
						  matrix_aux[boton_actual] += 2;
						}
					  }
					  else //para los impares (minusculas)
					  {
						if (matrix_aux[boton_actual] >= 7){
						  matrix_aux[boton_actual] = 1;
						}
						else
						{
						  matrix_aux[boton_actual] += 2;
						}
					  }
					}
				} // fin movimiento ciclico

				// si es la primera vez que toco ese boton tengo que posicionarme al principio, ya que la matriz tiene valor 0 hasta ese momento
				if (first_touch)
				{
				  if(mayus)
					matrix_aux[boton_actual] = 2;
				  else
					matrix_aux[boton_actual] = 1;
				  first_touch = 0;
				}
			}
			else // botones 0 y 1
			{
				if (matrix_aux[boton_actual] < 9)
					matrix_aux[boton_actual]++;
				else
					matrix_aux[boton_actual] = 1;
			}

			mensaje_buffer[cursor] = get_charmap(boton_actual,matrix_aux[boton_actual]);
			update_LCD(mensaje_buffer,1,cursor+1);
			conteo_salto = 0;
			} // FIN FUNCIONALIDAD BOTON

			// FUNCIONALIDAD CONFIRMAR LETRA
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
			} // FIN CONFIRMAR LETRA

			// FUNCIONALIDAD BOTON DERECHA
			if (boton_seleccionado == DERECHA)
			{
				//la segunda parte evita que te puedas mover a posiciones vacias sin tener letras atras
				if ((cursor < MAX_LENGTH_MESSAGE-1) && (mensaje_buffer[cursor+1] != '\0'))
					cursor++;
			} // FIN BOTON DERECHA

			//FUNCIONALIDAD SHIFT
			//ESTA OPCION NO TIENE QUE AFECTAR A LOS BOTONES 1,0, DERECHA, IZQUIERDA, BORRAR, CONFIRMAR_LETRA
			if (boton_seleccionado == SHIFT)
			{
			  shift = !shift;
			}

		  }// FIN BLOQUEO DE TECLAS

		  if (cursor > 0)
		  {
			// FUNCIONALIDAD BORRAR/CANCELAR
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
				{
					cursor--;
				}
				mensaje_buffer[cursor] = ' ';  //borrar caracter del LCD
				update_LCD(mensaje_buffer,1,cursor+1);
			} // FIN BORRAR

			  // FUNCIONALIDAD BOTON IZQUIERDA
			if (boton_seleccionado == IZQUIERDA)
			{
				cursor--;
			} // FIN BOTON IZQUIERDA
		  }

		  // FUNCIONALIDAD ENTER
		  if (boton_seleccionado == ENTER)
		  {
			conteo_salto = 0;
			flag_escribiendo = 0;
			matrix_aux[boton_actual] = 0;
			boton_actual = 'e';         	//empty
			boton_seleccionado = 'e';     //empty
			//mensaje_actual = mensaje_buffer;
			int i;
			for(i=0;i<=cursor;i++)
			{
				mensaje_actual[i] = mensaje_buffer[i];
			}
			if(!flag_inicio)
			{
				ActivateTask(PeriodicTask);
				flag_inicio = 1;
			}
		  } // FIN ENTER
		boton_seleccionado = 'e'; //limpio el boton para que no se vuelva a ingresar sin antes haber presionado
	}
}


void botones()
{
	uint16_t inputs = 0x00;
	uint16_t i;

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
	{
		boton_seleccionado = SHIFT;
	}

	if((inputs&0x02) == 0)
	{
		boton_seleccionado = IZQUIERDA;
	}

	if((inputs&0x04) == 0)
	{
		boton_seleccionado = DERECHA;
	}

	if((inputs&0x08) == 0)
	{
		boton_seleccionado = ENTER;
	}
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

   if((inputs&0x10)==0)  	//COLUMNA 1
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x10)==0){
		   boton_seleccionado = 1;
	   }

   }


   if((inputs&0x20)==0)		//COLUMNA 2
   {
	   for(i = 0; i<0xFFF; i++);	//anti rebote
	   if((inputs&0x20)==0){		//confirme boton
		   boton_seleccionado = 2;
	   }
   }

   if((inputs&0x40)==0)		//COLUMNA 3
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x40)==0){
		   boton_seleccionado = 3;
	   }
   }


	//FILA 2
   outputs = outputs & 0xFF7F;				//xxxx xx11 01xx xxxx
   outputs = outputs | 0x0340;
   ciaaPOSIX_write(fd_out, &outputs, 2);
   ciaaPOSIX_read(fd_in, &inputs, 1);

   if((inputs&0x10)==0)  	//COLUMNA 1
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x10)==0){
		   boton_seleccionado = 4;
	   }
   }

   if((inputs&0x20)==0)		//COLUMNA 2
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x20)==0){
		   boton_seleccionado = 5;
	   }
   }

   if((inputs&0x40)==0)		//COLUMNA 3
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x40)==0){
		   boton_seleccionado = 6;
	   }
   }

   //FILA 3
   outputs = outputs & 0xFEFF;				//xxxx xx10 11xx xxxx
   outputs = outputs | 0x02c0;
   ciaaPOSIX_write(fd_out, &outputs, 2);
   ciaaPOSIX_read(fd_in, &inputs, 1);

   if((inputs&0x10)==0)  	//COLUMNA 1
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x10)==0){
		   boton_seleccionado = 7;
	   }
   }

   if((inputs&0x20)==0)		//COLUMNA 2
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x20)==0){
		   boton_seleccionado = 8;
	   }
   }

   if((inputs&0x40)==0)		//COLUMNA 3
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x40)==0){
		   boton_seleccionado = 9;
	   }
   }

   //FILA 4
   outputs = outputs & 0xFDFF;				//xxxx xx01 11xx xxxx
   outputs = outputs | 0x01c0;
   ciaaPOSIX_write(fd_out, &outputs, 2);
   ciaaPOSIX_read(fd_in, &inputs, 1);

   if((inputs&0x10)==0)		//COLUMNA 1
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x10)==0){
		   boton_seleccionado = BORRAR;
	   }
   }

   if((inputs&0x20)==0)		//COLUMNA 2
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x20)==0){
		   boton_seleccionado = 0;
	   }
   }

   if((inputs&0x40)==0)		//COLUMNA 3
   {
	   for(i = 0; i<0xFFF; i++);
	   if((inputs&0x40)==0){
		   boton_seleccionado = CONFIRMAR;
	   }
   }

}

void botonera_reducida()
{

	uint16_t inputs = 0x00;

	ciaaPOSIX_read(fd_in, &inputs, 1);

	if((inputs&0x01) == 0)
	{
		boton_seleccionado = SHIFT;
	}

	if((inputs&0x08) == 0)
	{
		boton_seleccionado = ENTER;
	}

}
void escribir_reducido()
{
	static uint8_t valor = 0;
	conteo_funcionalidad++;
	if (conteo_funcionalidad == 3)
	{
		conteo_funcionalidad = 0;
		//usamos el boton shift de forma temporal como contador para que el usuario seleccione su opcion
		if (boton_seleccionado == SHIFT)
		{
			if (state == MODO)
			{
				if ((valor > 1) && (matrix_aux[1] > 1))
					matrix_aux[1] = 0;
			}
			else
			{
				if ((valor > 4) && (matrix_aux[1] > 4))
					matrix_aux[1] = 0;
			}
			valor = matrix_aux[1];
			mensaje_buffer[0] = get_charmap(1,++matrix_aux[1]);
			update_LCD(mensaje_buffer,1,1);
		}
		if (boton_seleccionado == ENTER)
		{
			switch (state){
				case VELOCIDAD:
				{
					speed = valor;
					matrix_aux[1] = 0;
					flag_cartel = 0;
					break;
				}
				case INTENSIDAD:
				{
					intensity = valor;
					matrix_aux[1] = 0;
					flag_cartel = 0;
					break;
				}

				case MODO:
				{
					mode = valor;
					matrix_aux[1] = 0;
					mensaje_buffer[0] = 0;
					flag_cartel = 0;
				    // Init para integrado MAX7219
				    Init_MAX7219();
					break;
				}
			}
			state++;
			update_LCD(" ",1,1);
		}
		boton_seleccionado = 'e';
	}
}

