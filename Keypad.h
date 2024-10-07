#ifndef KEYPAD_H_
#define KEYPAD_H_


#define DDR_KEYPAD  DDRL
#define PORT_KEYPAD PORTL
#define PIN_KEYPAD  PINL

const int	Key_1 = 0b11101110;
const int	Key_2 =	0b11011110;
const int	Key_3 =	0b10111110;
const int	Key_A =	0b01111110;

const int	Key_4 =	0b11101101;
const int	Key_5 =	0b11011101;
const int	Key_6 =	0b10111101;
const int	Key_B =	0b01111101;

const int	Key_7 =	0b11101011;
const int	Key_8 =	0b11011011;
const int	Key_9 =	0b10111011;
const int	Key_C =	0b01111011;

const int	Key_F =	0b11100111;
const int	Key_0 =	0b11010111;
const int	Key_E =	0b10110111;
const int	Key_D =	0b01110111;

unsigned char freePinFromKeyPad = 1;
unsigned char keyFromKeyPad = 1;
char pressedButton = 0;
int logicPressed = 0x00;

void initKeyPad() {
	DDR_KEYPAD = 0x0F;
	PORT_KEYPAD = 0xF0;
}

char readKeyFromPad4x4() {
	logicPressed = 0x00;
	return pressedButton;
}

int isButtonPressed() {
	
			PORTE |= (1 << 1);
			_delay_ms(200);
			PORTE = 0x00;
	
	if (freePinFromKeyPad == 1)
	{
		if (PIN_KEYPAD != 0xF0)
		{
			_delay_ms(50);
			freePinFromKeyPad = 0;
			keyFromKeyPad = 1;

			PORT_KEYPAD = 0b11111110;
			asm("nop");
			if (PORT_KEYPAD == PIN_KEYPAD)
			{
				PORT_KEYPAD = 0b11111101;
				asm("nop");
				if (PORT_KEYPAD == PIN_KEYPAD)
				{
					PORT_KEYPAD = 0b11111011;
					asm("nop");
					if (PORT_KEYPAD == PIN_KEYPAD)
					{
						PORT_KEYPAD = 0b11110111;
						asm("nop");
						if (PORT_KEYPAD == PIN_KEYPAD)
						keyFromKeyPad = 0;
					}
				}
			}
			if (keyFromKeyPad == 1)
			{
				if (PIN_KEYPAD == Key_1) pressedButton = '7';
				else if (PIN_KEYPAD == Key_2) pressedButton = '8';
				else if (PIN_KEYPAD == Key_3) pressedButton = '9';
				else if (PIN_KEYPAD == Key_4) pressedButton = '4';
				else if (PIN_KEYPAD == Key_5) pressedButton = '5';
				else if (PIN_KEYPAD == Key_6) pressedButton = '6';
				else if (PIN_KEYPAD == Key_7) pressedButton = '1';
				else if (PIN_KEYPAD == Key_8) pressedButton = '2';
				else if (PIN_KEYPAD == Key_9) pressedButton = '3';
				else if (PIN_KEYPAD == Key_0) pressedButton = '0';
				else if (PIN_KEYPAD == Key_A) pressedButton = 'A';
				else if (PIN_KEYPAD == Key_B) pressedButton = 'B';
				else if (PIN_KEYPAD == Key_C) pressedButton = 'C';
				else if (PIN_KEYPAD == Key_D) pressedButton = 'D';
				else if (PIN_KEYPAD == Key_E) pressedButton = 'E';
				else if (PIN_KEYPAD == Key_F) pressedButton = 'F';
				logicPressed = 0x01;
			}

			PORT_KEYPAD = 0xF0;
		}
	}
	else if (PIN_KEYPAD == 0xF0)
	{
		_delay_ms(200);
		freePinFromKeyPad = 1;
	}
	return logicPressed;
}


#endif