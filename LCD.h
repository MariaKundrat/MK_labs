#ifndef LCD_H_
#define LCD_H_

#define _0 0x30
#define _1 0x31
#define _2 0x32
#define _3 0x33
#define _4 0x34
#define _5 0x35
#define _6 0x36
#define _7 0x37
#define _8 0x38
#define _9 0x39
#define _Mes 0x3A

const int regis[] = {_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _Mes};


#define Data_PORT PORTC
#define Data_DDR DDRC
#define Data_PIN PINC

#define Command_PORT PORTC
#define Command_DDR	DDRC
#define Command_PIN PINC


#define DB4 0
#define DB5 1
#define DB6 2
#define DB7 3

#define E 4
#define RW 5
#define RS 6

#define ID 1
#define S 0

#define D 2
#define C 1
#define B 0

#define SL 3
#define RL 2

#define DL 4
#define N 3
#define F 2

#define BF 3

#define CGRAM_COM 0x40
#define DDRAM_COM 0x80

#define Clear_COM 0x01


void sentRequest(){
	Command_DDR |= (1 << E);
	
	Command_PORT |= (1 << E);
	_delay_us(0.15);
	Command_PORT &= ~(1 << E);
}

void initLCD4bit(){
	_delay_ms(50);
	Data_DDR |= (1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4);
	Data_PORT &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	
	Command_DDR |= (1 << RS) | (1 << RW) | (1 << E);
	Command_PORT &= ~((1 << RS) | (1 << RW) | (1 << E));

	Data_PORT |= (1 << DB5) | (1 << DB4);
	sentRequest();
	_delay_ms(5);
	sentRequest();
	_delay_us(120);
	sentRequest();
	_delay_us(50);
	
	Data_PORT = 0x00;
	Data_PORT |= (1 << DB5);
	
	sentRequest();
	_delay_us(50);
	
	sentRequest();
	
	Data_PORT &= (1 << DB5);
	
	Data_PORT |= (1 << DB7);
	
	sentRequest();
	_delay_us(50);
	
	Data_PORT = 0x00;
	sentRequest();
	Data_PORT |= (1 << DB7) | (1 << D) | (1 << C
	);
	sentRequest();
	_delay_us(50);
	
	Data_PORT = 0x00;
	sentRequest();
	Data_PORT |= (1 << DB6);
	sentRequest();
	_delay_us(50);
	
	Data_PORT = 0x00;
	sentRequest();
	Data_PORT |= (1 <<DB4);
	sentRequest();
	_delay_ms(50);
}

void BFS(){
	
	uint8_t temp = 0x00;
	Command_DDR &= ~(1 << RS);
	Command_DDR |= (1 << RW) | (1 << E);
	Data_DDR &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	Command_PORT |= (1 << RW);
	Data_PORT &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	
	do {
		_delay_ms(50);
		Command_PORT |=(1 << E);
		_delay_us(0.15);
		temp = ((Data_PIN >> DB7) & 0x01);
		Command_PORT &= ~(1 << E);
		
		sentRequest();
	} while (temp);
}


void LCD_write_comm(unsigned char command){
	_delay_ms(5);
	
	Data_DDR |= (1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4);
	Data_PORT &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	
	Command_DDR |= (1 << RS) | (1 << RW) | (1 << E);
	Command_PORT &= ~((1 << RS) | (1 << RW) | (1 << E));
	
	Data_PORT |= (command >> 4);
	sentRequest();
	Data_PORT &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	Data_PORT |= (command & 0x0F);
	sentRequest();
	Data_PORT &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	_delay_ms(5);
}

void LCD_write_symphol(unsigned char Letter){
	_delay_ms(5);
	
	Data_DDR |= (1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4);
	Data_PORT &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	
	Command_DDR |= (1 << RS) | (1 << RW) | (1 << E);
	Command_PORT &= ~((1 << RS) | (1 << RW) | (1 << E));
	
	Command_PORT |= (1 << RS);
	
	Data_PORT |= (Letter >> 4);
	sentRequest();
	Data_PORT &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	Data_PORT |= (Letter  & 0x0F);
	sentRequest();
	Data_PORT &= ~((1 << DB7) | (1 << DB6) | (1 << DB5) | (1 << DB4));
	_delay_ms(5);
}


#endif