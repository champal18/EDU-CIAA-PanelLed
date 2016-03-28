#include "LedControl.h"
#include "lcd.h"

 // define max7219 registers
const uint8_t MAX7219_REG_NOOP        = 0;
const uint8_t MAX7219_REG_DIGIT0      = 1;
const uint8_t MAX7219_REG_DIGIT1      = 2;
const uint8_t MAX7219_REG_DIGIT2      = 3;
const uint8_t MAX7219_REG_DIGIT3      = 4;
const uint8_t MAX7219_REG_DIGIT4      = 5;
const uint8_t MAX7219_REG_DIGIT5      = 6;
const uint8_t MAX7219_REG_DIGIT6      = 7;
const uint8_t MAX7219_REG_DIGIT7      = 8;
const uint8_t MAX7219_REG_DECODEMODE  = 9;
const uint8_t MAX7219_REG_INTENSITY   = 10;
const uint8_t MAX7219_REG_SCANLIMIT   = 11;
const uint8_t MAX7219_REG_SHUTDOWN    = 12;
const uint8_t MAX7219_REG_DISPLAYTEST = 15;

uint8_t status[64];

uint8_t speed;
uint8_t intensity;
uint8_t mode;

extern uint8_t mensaje_actual[17];

void clearDisplay() 
{
	int i;
    for(i=0;i<8;i++)
    {
	   sendByte(i+1,0);
    }
}

void setRow(int row, uint8_t value)
{
    if(row<0 || row>7)
	   return;
    
    status[row]=value;
    sendByte(row+1,status[row]);
}

void printChar(int pos, char c)
{  
	uint8_t sRow[7];
	int i=0;

	sRow[0] = 0x00;
	sRow[6] = 0x00;

	switch (c){
	case ' ':
		sRow[1] = 0x00;
		sRow[2] = 0x00;
		sRow[3] = 0x00;
		sRow[4] = 0x00;
		sRow[5] = 0x00;
		break;

	case '!':
		sRow[1] = 0x00;
		sRow[2] = 0x00;
		sRow[3] = 0x2f;
		sRow[4] = 0x00;
		sRow[5] = 0x00;
		break;

	case '"':
		sRow[1] = 0x00;
		sRow[2] = 0x07;
		sRow[3] = 0x00;
		sRow[4] = 0x07;
		sRow[5] = 0x00;
		break;

	case '#':
		sRow[1] = 0x14;
		sRow[2] = 0x7f;
		sRow[3] = 0x14;
		sRow[4] = 0x7f;
		sRow[5] = 0x14;
		break;

	case '$':
		sRow[1] = 0x24;
		sRow[2] = 0x2a;
		sRow[3] = 0x7f;
		sRow[4] = 0x2a;
		sRow[5] = 0x12;
		break;

	case '%':
		sRow[1] = 0x62;
		sRow[2] = 0x64;
		sRow[3] = 0x08;
		sRow[4] = 0x13;
		sRow[5] = 0x23;
		break;

	case '&':
		sRow[1] = 0x36;
		sRow[2] = 0x49;
		sRow[3] = 0x55;
		sRow[4] = 0x22;
		sRow[5] = 0x50;
		break;

	case '(':
		sRow[1] = 0x00;
		sRow[2] = 0x1c;
		sRow[3] = 0x22;
		sRow[4] = 0x41;
		sRow[5] = 0x00;
		break;

	case ')':
		sRow[1] = 0x00;
		sRow[2] = 0x41;
		sRow[3] = 0x22;
		sRow[4] = 0x1c;
		sRow[5] = 0x00;
	break;

	case '*':
		sRow[1] = 0x14;
		sRow[2] = 0x08;
		sRow[3] = 0x3E;
		sRow[4] = 0x08;
		sRow[5] = 0x14;
	break;

	case '+':
		sRow[1] = 0x08;
		sRow[2] = 0x08;
		sRow[3] = 0x3E;
		sRow[4] = 0x08;
		sRow[5] = 0x08;
	break;

	case ',':
		sRow[1] = 0x00;
		sRow[2] = 0xA0;
		sRow[3] = 0x60;
		sRow[4] = 0x00;
		sRow[5] = 0x00;
	break;

	case '-':
		sRow[1] = 0x08;
		sRow[2] = 0x08;
		sRow[3] = 0x08;
		sRow[4] = 0x08;
		sRow[5] = 0x08;
	break;

	case '.':
		sRow[1] = 0x00;
		sRow[2] = 0x60;
		sRow[3] = 0x60;
		sRow[4] = 0x00;
		sRow[5] = 0x00;
	break;

	case '/':
		sRow[1] = 0x20;
		sRow[2] = 0x10;
		sRow[3] = 0x08;
		sRow[4] = 0x04;
		sRow[5] = 0x02;
	break;

	case '0':
		sRow[1] = 0x3E;
		sRow[2] = 0x51;
		sRow[3] = 0x49;
		sRow[4] = 0x45;
		sRow[5] = 0x3E;
	break;

	case '1':
		sRow[1] = 0x00;
		sRow[2] = 0x42;
		sRow[3] = 0x7F;
		sRow[4] = 0x40;
		sRow[5] = 0x00;
	break;

	case '2':
		sRow[1] = 0x42;
		sRow[2] = 0x61;
		sRow[3] = 0x51;
		sRow[4] = 0x49;
		sRow[5] = 0x46;
	break;

	case '3':
		sRow[1] = 0x21;
		sRow[2] = 0x41;
		sRow[3] = 0x45;
		sRow[4] = 0x4B;
		sRow[5] = 0x31;
	break;

	case '4':
		sRow[1] = 0x18;
		sRow[2] = 0x14;
		sRow[3] = 0x12;
		sRow[4] = 0x7F;
		sRow[5] = 0x10;
	break;

	case '5':
		sRow[1] = 0x27;
		sRow[2] = 0x45;
		sRow[3] = 0x45;
		sRow[4] = 0x45;
		sRow[5] = 0x39;
	break;

	case '6':
		sRow[1] = 0x3C;
		sRow[2] = 0x4A;
		sRow[3] = 0x49;
		sRow[4] = 0x49;
		sRow[5] = 0x30;
	break;

	case '7':
		sRow[1] = 0x01;
		sRow[2] = 0x71;
		sRow[3] = 0x09;
		sRow[4] = 0x05;
		sRow[5] = 0x03;
	break;

	case '8':
		sRow[1] = 0x36;
		sRow[2] = 0x49;
		sRow[3] = 0x49;
		sRow[4] = 0x49;
		sRow[5] = 0x36;
	break;

	case '9':
		sRow[1] = 0x06;
		sRow[2] = 0x49;
		sRow[3] = 0x49;
		sRow[4] = 0x29;
		sRow[5] = 0x1E;
	break;

	case ':':
		sRow[1] = 0x00;
		sRow[2] = 0x36;
		sRow[3] = 0x36;
		sRow[4] = 0x00;
		sRow[5] = 0x00;
	break;

	case ';':
		sRow[1] = 0x00;
		sRow[2] = 0x56;
		sRow[3] = 0x36;
		sRow[4] = 0x00;
		sRow[5] = 0x00;
	break;

	case '<':
		sRow[1] = 0x08;
		sRow[2] = 0x14;
		sRow[3] = 0x22;
		sRow[4] = 0x41;
		sRow[5] = 0x00;
	break;

	case '=':
		sRow[1] = 0x14;
		sRow[2] = 0x14;
		sRow[3] = 0x14;
		sRow[4] = 0x14;
		sRow[5] = 0x14;
	break;

	case '>':
		sRow[1] = 0x00;
		sRow[2] = 0x41;
		sRow[3] = 0x22;
		sRow[4] = 0x14;
		sRow[5] = 0x08;
	break;

	case '?':
		sRow[1] = 0x02;
		sRow[2] = 0x01;
		sRow[3] = 0x51;
		sRow[4] = 0x09;
		sRow[5] = 0x06;
	break;

	case '@':
		sRow[1] = 0x32;
		sRow[2] = 0x49;
		sRow[3] = 0x59;
		sRow[4] = 0x51;
		sRow[5] = 0x3E;
	break;

	case 'A':
		sRow[1] = 0x7C;
		sRow[2] = 0x12;
		sRow[3] = 0x11;
		sRow[4] = 0x12;
		sRow[5] = 0x7C;
	break;

	case 'B':
		sRow[1] = 0x7F;
		sRow[2] = 0x49;
		sRow[3] = 0x49;
		sRow[4] = 0x49;
		sRow[5] = 0x36;
	break;

	case 'C':
		sRow[1] = 0x3E;
		sRow[2] = 0x41;
		sRow[3] = 0x41;
		sRow[4] = 0x41;
		sRow[5] = 0x22;
	break;

	case 'D':
		sRow[1] = 0x7F;
		sRow[2] = 0x41;
		sRow[3] = 0x41;
		sRow[4] = 0x22;
		sRow[5] = 0x1C;
	break;

	case 'E':
		sRow[1] = 0x7F;
		sRow[2] = 0x49;
		sRow[3] = 0x49;
		sRow[4] = 0x49;
		sRow[5] = 0x41;
	break;

	case 'F':
		sRow[1] = 0x7F;
		sRow[2] = 0x09;
		sRow[3] = 0x09;
		sRow[4] = 0x09;
		sRow[5] = 0x01;
	break;

	case 'G':
		sRow[1] = 0x3E;
		sRow[2] = 0x41;
		sRow[3] = 0x49;
		sRow[4] = 0x49;
		sRow[5] = 0x7A;
	break;

	case 'H':
		sRow[1] = 0x7F;
		sRow[2] = 0x08;
		sRow[3] = 0x08;
		sRow[4] = 0x08;
		sRow[5] = 0x7F;
	break;

	case 'I':
		sRow[1] = 0x00;
		sRow[2] = 0x41;
		sRow[3] = 0x7F;
		sRow[4] = 0x41;
		sRow[5] = 0x00;
	break;

	case 'J':
		sRow[1] = 0x20;
		sRow[2] = 0x40;
		sRow[3] = 0x41;
		sRow[4] = 0x3F;
		sRow[5] = 0x01;
	break;

	case 'K':
		sRow[1] = 0x7F;
		sRow[2] = 0x08;
		sRow[3] = 0x14;
		sRow[4] = 0x22;
		sRow[5] = 0x41;
	break;

	case 'L':
		sRow[1] = 0x7F;
		sRow[2] = 0x40;
		sRow[3] = 0x40;
		sRow[4] = 0x40;
		sRow[5] = 0x40;
	break;

	case 'M':
		sRow[1] = 0x7F;
		sRow[2] = 0x02;
		sRow[3] = 0x0C;
		sRow[4] = 0x02;
		sRow[5] = 0x7F;
	break;

	case 'N':
		sRow[1] = 0x7F;
		sRow[2] = 0x04;
		sRow[3] = 0x08;
		sRow[4] = 0x10;
		sRow[5] = 0x7F;
	break;

	case 'O':
		sRow[1] = 0x3E;
		sRow[2] = 0x41;
		sRow[3] = 0x41;
		sRow[4] = 0x41;
		sRow[5] = 0x3E;
	break;

	case 'P':
		sRow[1] = 0x7F;
		sRow[2] = 0x09;
		sRow[3] = 0x09;
		sRow[4] = 0x09;
		sRow[5] = 0x06;
	break;

	case 'Q':
		sRow[1] = 0x3E;
		sRow[2] = 0x41;
		sRow[3] = 0x51;
		sRow[4] = 0x21;
		sRow[5] = 0x5E;
	break;

	case 'R':
		sRow[1] = 0x7F;
		sRow[2] = 0x09;
		sRow[3] = 0x19;
		sRow[4] = 0x29;
		sRow[5] = 0x46;
	break;

	case 'S':
		sRow[1] = 0x46;	// 0100 0110
		sRow[2] = 0x49; // 0100 1001
		sRow[3] = 0x49; // 0100 1001
		sRow[4] = 0x49; // 0100 1001
		sRow[5] = 0x31;	// 0011 0001
	break;

	case 'T':
		sRow[1] = 0x01;
		sRow[2] = 0x01;
		sRow[3] = 0x7F;
		sRow[4] = 0x01;
		sRow[5] = 0x01;
	break;

	case 'U':
		sRow[1] = 0x3F;
		sRow[2] = 0x40;
		sRow[3] = 0x40;
		sRow[4] = 0x40;
		sRow[5] = 0x3F;
	break;

	case 'V':
		sRow[1] = 0x1F;
		sRow[2] = 0x20;
		sRow[3] = 0x40;
		sRow[4] = 0x20;
		sRow[5] = 0x1F;
	break;

	case 'W':
		sRow[1] = 0x3F;
		sRow[2] = 0x40;
		sRow[3] = 0x38;
		sRow[4] = 0x40;
		sRow[5] = 0x3F;
	break;

	case 'X':
		sRow[1] = 0x63;
		sRow[2] = 0x14;
		sRow[3] = 0x08;
		sRow[4] = 0x14;
		sRow[5] = 0x63;
	break;

	case 'Y':
		sRow[1] = 0x07;
		sRow[2] = 0x08;
		sRow[3] = 0x70;
		sRow[4] = 0x08;
		sRow[5] = 0x07;
	break;

	case 'Z':
		sRow[1] = 0x61;
		sRow[2] = 0x51;
		sRow[3] = 0x49;
		sRow[4] = 0x45;
		sRow[5] = 0x43;
	break;

	case '[':
		sRow[1] = 0x00;
		sRow[2] = 0x7F;
		sRow[3] = 0x41;
		sRow[4] = 0x41;
		sRow[5] = 0x00;
	break;

	case '\\':
		sRow[1] = 0x55;
		sRow[2] = 0xAA;
		sRow[3] = 0x55;
		sRow[4] = 0xAA;
		sRow[5] = 0x55;
	break;

	case ']':
		sRow[1] = 0x00;
		sRow[2] = 0x41;
		sRow[3] = 0x41;
		sRow[4] = 0x7F;
		sRow[5] = 0x00;
	break;

	case '^':
		sRow[1] = 0x04;
		sRow[2] = 0x02;
		sRow[3] = 0x01;
		sRow[4] = 0x02;
		sRow[5] = 0x04;
	break;

	case '_':
		sRow[1] = 0x40;
		sRow[2] = 0x40;
		sRow[3] = 0x40;
		sRow[4] = 0x40;
		sRow[5] = 0x40;
	break;

	case '`':
		sRow[1] = 0x00;
		sRow[2] = 0x03;
		sRow[3] = 0x05;
		sRow[4] = 0x00;
		sRow[5] = 0x00;
	break;

	case 'a':
		sRow[1] = 0x20;
		sRow[2] = 0x54;
		sRow[3] = 0x54;
		sRow[4] = 0x54;
		sRow[5] = 0x78;
	break;

	case 'b':
		sRow[1] = 0x7F;
		sRow[2] = 0x48;
		sRow[3] = 0x44;
		sRow[4] = 0x44;
		sRow[5] = 0x38;
	break;

	case 'c':
		sRow[1] = 0x38;
		sRow[2] = 0x44;
		sRow[3] = 0x44;
		sRow[4] = 0x44;
		sRow[5] = 0x20;
	break;

	case 'd':
		sRow[1] = 0x38;
		sRow[2] = 0x44;
		sRow[3] = 0x44;
		sRow[4] = 0x48;
		sRow[5] = 0x7F;
	break;

	case 'e':
		sRow[1] = 0x38;
		sRow[2] = 0x54;
		sRow[3] = 0x54;
		sRow[4] = 0x54;
		sRow[5] = 0x18;
	break;

	case 'f':
		sRow[1] = 0x08;
		sRow[2] = 0x7E;
		sRow[3] = 0x09;
		sRow[4] = 0x01;
		sRow[5] = 0x02;
	break;

	case 'g':
		sRow[1] = 0x18;
		sRow[2] = 0xA4;
		sRow[3] = 0xA4;
		sRow[4] = 0xA4;
		sRow[5] = 0x7C;
	break;

	case 'h':
		sRow[1] = 0x7F;
		sRow[2] = 0x08;
		sRow[3] = 0x04;
		sRow[4] = 0x04;
		sRow[5] = 0x78;
	break;

	case 'i':
		sRow[1] = 0x00;
		sRow[2] = 0x44;
		sRow[3] = 0x7D;
		sRow[4] = 0x40;
		sRow[5] = 0x00;
	break;

	case 'j':
		sRow[1] = 0x40;
		sRow[2] = 0x80;
		sRow[3] = 0x84;
		sRow[4] = 0x7D;
		sRow[5] = 0x00;
	break;

	case 'k':
		sRow[1] = 0x7F;
		sRow[2] = 0x10;
		sRow[3] = 0x28;
		sRow[4] = 0x44;
		sRow[5] = 0x00;
	break;

	case 'l':
		sRow[1] = 0x00;
		sRow[2] = 0x41;
		sRow[3] = 0x7F;
		sRow[4] = 0x40;
		sRow[5] = 0x00;
	break;

	case 'm':
		sRow[1] = 0x7C;
		sRow[2] = 0x04;
		sRow[3] = 0x18;
		sRow[4] = 0x04;
		sRow[5] = 0x78;
	break;

	case 'n':
		sRow[1] = 0x7C;
		sRow[2] = 0x08;
		sRow[3] = 0x04;
		sRow[4] = 0x04;
		sRow[5] = 0x78;
	break;

	case 'o':
		sRow[1] = 0x38;
		sRow[2] = 0x44;
		sRow[3] = 0x44;
		sRow[4] = 0x44;
		sRow[5] = 0x38;
	break;

	case 'p':
		sRow[1] = 0xFC;
		sRow[2] = 0x24;
		sRow[3] = 0x24;
		sRow[4] = 0x24;
		sRow[5] = 0x18;
	break;

	case 'q':
		sRow[1] = 0x18;
		sRow[2] = 0x24;
		sRow[3] = 0x24;
		sRow[4] = 0x18;
		sRow[5] = 0xFC;
	break;

	case 'r':
		sRow[1] = 0x7C;
		sRow[2] = 0x08;
		sRow[3] = 0x04;
		sRow[4] = 0x04;
		sRow[5] = 0x08;
	break;

	case 's':
		sRow[1] = 0x48;
		sRow[2] = 0x54;
		sRow[3] = 0x54;
		sRow[4] = 0x54;
		sRow[5] = 0x20;
	break;

	case 't':
		sRow[1] = 0x04;
		sRow[2] = 0x3F;
		sRow[3] = 0x44;
		sRow[4] = 0x40;
		sRow[5] = 0x20;
	break;

	case 'u':
		sRow[1] = 0x3C;
		sRow[2] = 0x40;
		sRow[3] = 0x40;
		sRow[4] = 0x20;
		sRow[5] = 0x7C;
	break;

	case 'v':
		sRow[1] = 0x1C;
		sRow[2] = 0x20;
		sRow[3] = 0x40;
		sRow[4] = 0x20;
		sRow[5] = 0x1C;
	break;

	case 'w':
		sRow[1] = 0x3C;
		sRow[2] = 0x40;
		sRow[3] = 0x30;
		sRow[4] = 0x40;
		sRow[5] = 0x3C;
	break;

	case 'x':
		sRow[1] = 0x44;
		sRow[2] = 0x28;
		sRow[3] = 0x10;
		sRow[4] = 0x28;
		sRow[5] = 0x44;
	break;

	case 'y':
		sRow[1] = 0x1C;
		sRow[2] = 0xA0;
		sRow[3] = 0xA0;
		sRow[4] = 0xA0;
		sRow[5] = 0x7C;
	break;

	case 'z':
		sRow[1] = 0x44;
		sRow[2] = 0x64;
		sRow[3] = 0x54;
		sRow[4] = 0x4C;
		sRow[5] = 0x44;
	break;

	case '{':
		sRow[1] = 0x00;
		sRow[2] = 0x10;
		sRow[3] = 0x7C;
		sRow[4] = 0x82;
		sRow[5] = 0x00;
	break;

	case '|':
		sRow[1] = 0x00;
		sRow[2] = 0x00;
		sRow[3] = 0xFF;
		sRow[4] = 0x00;
		sRow[5] = 0x00;
	break;

	case '}':
		sRow[1] = 0x00;
		sRow[2] = 0x82;
		sRow[3] = 0x7C;
		sRow[4] = 0x10;
		sRow[5] = 0x00;
	break;

	case 'º':
		sRow[1] = 0x00;
		sRow[2] = 0x06;
		sRow[3] = 0x09;
		sRow[4] = 0x09;
		sRow[5] = 0x06;
	break;
	}

	for (i=0; i<7; i++)
	{
	setRow(pos+i, sRow[i]);
	}

}

void printString(int pos, char string[]){
  
  int i=0;
  
  while (string[i] != '\0')
  {
    printChar(6*i+pos, string[i]);
    i++;
  }
  
}


void printStringScroll(int pos, char string[], int tDelay, char sentido)
{  
    int i=0, c=0; 
    while (string[c] != '\0')
    {
        c++;
    }
    if (sentido == '<')
    {  
        for (i=0; i<((c*6)+1); i++)
        {
          printString(-i+pos, string);
          LCD_delay_ms(tDelay);
        }
    }
    else    
        if (sentido == '>')
        {
            for (i=0; i<((c*6)+1); i++)
            {
              printString((-(c*6)+i)+pos, string);
              LCD_delay_ms(tDelay);
            }
        }
}

//////////////////////////  MIS FUNCIONES  /////////////////////////////////////

void sendByte(const uint8_t reg, const uint8_t data)
{
	uint8_t *buffer;
	int aux = (reg << 8) | data ;  // Buffer = 0000 0XXX (adress) 0000 0000 (data)
	buffer = &aux;
	Chip_SSP_WriteFrames_Blocking(LPC_SSP1, buffer, 1);

}	// end of sendByte


// Setup del MAX7219
void Init_MAX7219(void)
{
	int i;
	for(i=1;i<65;i++)
		status[i]=0x00;

	sendByte (MAX7219_REG_DISPLAYTEST, 0); // no display test
	sendByte (MAX7219_REG_SCANLIMIT, 7);   // show all 8 digits
	sendByte (MAX7219_REG_DECODEMODE, 0);  // using an led matrix (not digits)

	// clear display
	/*for (col = 0; col < 8; col++)
		sendByte ((uint8_t)(col + 1), 0);*/
	clearDisplay();
	sendByte (MAX7219_REG_SHUTDOWN, 0);   // not in shutdown mode (ie. start it up)

	//////////////////
	sendByte (MAX7219_REG_SHUTDOWN, 1);   // not in shutdown mode (ie. start it up)
	sendByte (MAX7219_REG_INTENSITY, intensity*3);  // character intensity: range: 0 to 15

	// clear display
	/*for (col = 0; col < 8; col++)
		sendByte ((uint8_t)(col + 1), 0);*/
	clearDisplay();

}	// end of Init_MAX7219

void msjScroll()
{
	int i = 0;

	switch(mode)
	{
		case 0:
		{
			for(i=0;i<(sizeof(mensaje_actual)/sizeof(uint8_t))-1;i++)
			{
				clearDisplay();
				printChar(2,mensaje_actual[i]);
				LCD_delay_ms(700); //speed*X con X numero a calcular
			}
			break;
		}
		case 1:
		{
			clearDisplay();     //borrar pantalla '0'
			printStringScroll(1, mensaje_actual, (int)(40 + speed*20), '<');
			LCD_delay_ms(250);
			clearDisplay();     //borrar pantalla '0'
			break;
		}
		case 2:
		{
			clearDisplay();     //borrar pantalla '0'
			printStringScroll(1, mensaje_actual, (int)(40 + speed*20), '<');
			LCD_delay_ms(250);
			printStringScroll(1, mensaje_actual, (int)(40 + speed*20), '>');
			LCD_delay_ms(250);
			clearDisplay();     //borrar pantalla '0'
			break;
		}
	}
}
