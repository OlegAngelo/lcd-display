#include <xc.h> // include file for xc8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void delay(int time)
{
	int i, j;
	for (i = 0; i < time; i++)
	{
		for (j = 0; j < 100; j++);
	}
}

void instCtrl(unsigned char data)
{
	PORTC = data; // load data to port c since my port c is the output pins
	RB0 = 0;	  // set RS to 0 (instruction reg)
	RB1 = 0;	  // set RW to 0 (write)
	RB2 = 1;	  // set E to 1
	delay(5);
	RB2 = 0; // set E to 0 (strobe)
}

void initLCD()
{
	delay(50);

	instCtrl(0x3C); // function set: 8-bit; dual-line
	instCtrl(0x38); // display off
	instCtrl(0x01); // display clear
	instCtrl(0x06); // entry mode: increment; shift off
	instCtrl(0x0E); // display on; cursor on; blink off
}

void dataCtrl(unsigned char data)
{
	PORTC = data; // load data to PORT C since its our output
	RB0 = 1;	  // set RS to 1 (data reg)
	RB1 = 0;	  // set RW to 0 (write)
	RB2 = 1;	  // set E to 1
	delay(5);
	RB2 = 0; // set E to 0 (strobe)
}

void clearDataResetCursor() {
	instCtrl(0x01); //clear data
	delay(5);
	instCtrl(0x02); // move cursr
	delay(5);
}	

void main()
{
	TRISC = 0x00; // all ports C are output to LCD
	TRISB = 0x00; // Set PORTB as output
	TRISD = 0xFF; // all ports D are input from IC

	unsigned char keypadData; // data from keypad

	initLCD();
	instCtrl(0x01);
	
	int charCount = 0;
	
	while (1)
	{
		if (RD4 == 1) // Check if keypad is high (pressed)
		{
			charCount++;
			
			if(charCount > 40) { // expecting a 20x2 LCD can display 40 char at once
				clearDataResetCursor();
				charCount = 0;
			}	

			keypadData = PORTD & 0x0F; // collect data in PORTD and mask
			delay(5);
		
		
			switch (keypadData)
			{
			case 0x00:
				dataCtrl('1');
				break;
			case 0x01:
				dataCtrl('2');
				break;
			case 0x02:
				dataCtrl('3');
				break;
			case 0x04:
				dataCtrl('4');
				break;
			case 0x05:
				dataCtrl('5');
				break;
			case 0x06:
				dataCtrl('6');
				break;
			case 0x08:
				dataCtrl('7');
				break;
			case 0x09:
				dataCtrl('8');
				break;
			case 0x0A:
				dataCtrl('9');
				break;
			case 0x0C:
				dataCtrl('*');
				break;
			case 0x0D:
				dataCtrl('0');
				break;
			case 0x0E:
				dataCtrl('#');
				break;
			default:
				dataCtrl(' ');
			}
		
			while (RD4 == 1);
		}
	}
}
