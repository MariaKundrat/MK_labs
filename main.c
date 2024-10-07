#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "Keypad.h"
#include "Timer.h"

#define B_DDR DDRE
#define B_PORT PORTE
#define B_PIN 1

struct Time Atimer ={0, 0, 10};
struct Time Btimer = {0, 42, 0};
	int StatusA = 0x00;
	int StatusB = 0x00;
	int Sig= 0;

void Signal(){
	B_DDR |= (1 << B_PIN);
		B_PORT |= (1 << B_PIN);
		_delay_ms(100);
}

void TimerOutLCD(char firstSt, struct Time obj, int line ){
	
	if(line == 2){
		LCD_write_comm(0b11000000);
	}
	else{
		LCD_write_comm(Clear_COM);
	}
	
	
	getTime(&obj);
	LCD_write_symphol(firstSt);
	LCD_write_symphol(0x3A);
	LCD_write_symphol(0x14);
	
	
	for (int i = 7; i >= 0; i-- ){
		LCD_write_symphol(obj.strList[i]);
	}
	
}

void Timer_all_out(){
		TimerOutLCD(0x41, Atimer, 1);
		TimerOutLCD(0x42, Btimer, 2);
}

ISR(TIMER1_COMPA_vect){
	
	if(StatusA){
		if(TimerWork(&Atimer) == 0x00){
			Sig = 10;
			StatusA = 0x00;
		}
	}
	
	if (StatusB){
		if(TimerWork(&Btimer) == 0x00){
			Sig = 10;
			StatusB = 0x00;
		}
	}
	
	TimerOutLCD(0x41, Atimer, 1);
	TimerOutLCD(0x42, Btimer, 2);
	
	
	if(Sig > 0 ){
		PORTE |= (1 << 1);
		_delay_ms(200);
		PORTE = 0x00;		
		Sig --;
	}
}


void TimerSettup( char type){
	SREG = 0x00;
	int arr[6] = {0, 0, 0, 0, 0, 0};
	int itter = 0;
	DDRA = 0xFF;
	
	
	while(1){
		
		
		if (isButtonPressed()){
			if(itter >= 6){
				itter = 0;
			}
			
			if (readKeyFromPad4x4() == 'F'){
				SREG |= (1 <<7);
				break;
			}else if (readKeyFromPad4x4() == 'E'){
				PORTA = 0xFF;
				int h, m, s;
				h = arr[5]*10 + arr[4];
				m = arr[3]*10 + arr[2];
				s = arr[1]*10 + arr[0];
				
				
				if(type == 'A'){
					setTimer(&Atimer, h, m, s);
				}else{
					setTimer(&Btimer, h, m, s);
				}
				
				Timer_all_out();
				
				SREG |= (1 <<7);
				break;
			}else if (readKeyFromPad4x4() == '0'){
				
				arr[itter] = 0;
				itter++;
			}else if (readKeyFromPad4x4() == '1'){
				arr[itter] = 1;
				itter++;
			}else if (readKeyFromPad4x4() == '2'){
				arr[itter] = 2;
				itter++;
			}else if (readKeyFromPad4x4() == '3'){
				arr[itter] = 3;
				itter++;
			}else if (readKeyFromPad4x4() == '4'){
				arr[itter] = 4;
				itter++;
			}else if (readKeyFromPad4x4() == '5'){
				arr[itter] = 5;
				itter++;
			}else if (readKeyFromPad4x4() == '6'){
				arr[itter] = 6;
				itter++;
			}else if (readKeyFromPad4x4() == '7'){
				arr[itter] = 7;
				itter++;
			}else if (readKeyFromPad4x4() == '8'){
				arr[itter] = 8;
				itter++;
			}else if (readKeyFromPad4x4() == '9'){
				arr[itter] = 9;
				itter++;
			}
			else{
			}
			
			LCD_write_comm(Clear_COM);
			for(int i=5 ; i >= 0; i-- ){
				int Symphol = 0x30 + arr[i];
				LCD_write_symphol(Symphol);
			}
		}
	}
}

int main(void)
{	
	
	DDRE = 0xFF;
	
	DDRB = 0xFF;
	PORTB = 0xFF;
	
	TCCR1A = 0x00;
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
	TIMSK1 = (1 << OCIE1A);
	OCR1A = 0x3D08;
	
	initLCD4bit();
	
	initKeyPad();
	
	TimerOutLCD(0x41, Atimer, 1);
	TimerOutLCD(0x42, Btimer, 2);
	
	SREG |= (1 << 7);
	
	setTimer(&Btimer, 67, 70, 50);
	
	while (1){
		if (StatusA | StatusB| Sig){
			TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
		}
		else{
			TCCR1B &= ~((1 << WGM12) | (1 << CS12) | (1 << CS10));
		}
		if (isButtonPressed()){
			
			if (readKeyFromPad4x4() == 'C'){// C
				if (StatusA){
					setNull(&Atimer);
					StatusA = 0x00;
				}
				else{
					StatusA = 0x01;
				}
			}
			else if(readKeyFromPad4x4() == 'D'){// D
				if (StatusB){
					setNull(&Btimer);
					StatusB = 0x00;
				}
				else{
					StatusB = 0x01;
				}
			}
			else if (readKeyFromPad4x4() == 'A'){ //A
				StatusA = 0x00;
				TimerSettup('A');
			}
			else if (readKeyFromPad4x4() == 'B'){ //A
				StatusB = 0x00;
				TimerSettup('B');
			}
			else if (readKeyFromPad4x4() == 'F'){
				Sig = 0;
			}	
		}
	}
}


