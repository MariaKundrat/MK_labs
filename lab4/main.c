#define F_CPU 7372800
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Segm.h"

#define SIG_PORT PORTC
#define SIG_DDR  DDRC
#define SIG_PIN  4
								
#define BTN_PORT PORTD
#define BTN_DDR  DDRD
#define BTN_PIN	 PIND

#define BTN_1	 3
#define BTN_2	 4
#define BTN_3	 5
#define BTN_4	 6

const int arr[10] = {S0, S1, S2, S3, S4, S5, S6, S7, S8, S9};
	
#include "Timer.h"

struct Time timerA = {0, 0, 5};
struct Time timerB = {0, 0, 10};
	
int ActTimer = 0x00;
	
struct Time *nTimer = &timerB;
	
int counter = 40;
int sigCount = 0;
unsigned int ms = 0;
unsigned int ms1 = 0;
unsigned int ms2 = 0;
unsigned int ms3 = 0;
unsigned int ms4 = 0;
unsigned int sig = 0;
unsigned int sigU = 0;

int i = 0;
int p = 0;
	
int StatusA = 0x01;
int StatusB = 0x00;

ISR(TIMER0_COMPA_vect){
	if(counter > 0){
		counter --;
		}else{
		counter = 40;
		
		if(StatusA){
			if(!TimerWork(&timerA)){
				StatusA = 0x00;
				sigCount = 10;
			}
		}
		if (StatusB){
			if (!TimerWork(&timerB)){
				StatusB = 0x00;
				sigCount = 10;
			}
		}
	}
}

ISR(TIMER1_COMPA_vect){
	ms ++;
	if (ms == 60001){
		ms = 0;
		ms1 = 0;
		ms2 = 0;
		ms3 = 0;
		ms4 = 0;
		sig = 0; 
		sigU = 0;
	}
}

int main(void)
{
	SEG_DDR = 0xFF;
	SEG_PORT = 0x00;
	
	V_DDR = 0xFF;
	V_PORT = 0x00;
	
	TCCR1B |= (1 << WGM12) | (1 << CS11); 
	OCR1A = 921; 
	TIMSK1 |= (1 << OCIE1A); 
    
	DDRA = 0xFF;
	PORTA = 0x40;
	
	DDRB = 0xFF;
	PORTB = segGet(0);
	BTN_DDR = 0x00;
	BTN_PORT = 0xFF;
	
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS02) | (1 << CS00);
	OCR0A = 180;
	TIMSK0 |= (1 << OCIE0A);
	
	SREG = 0x80;

    while (1){
	    PrintOnSeg();
	    
	    if (!(BTN_PIN & (1 << BTN_1)) ){
		    if(ms - ms1 >= 300){
				sigCount = 2;
			    ms1 = ms;

				if (ActTimer == 0x00){
					addH(&timerA);
				} 
				else{
					addH(&timerB);
				}
		    }
	    }
		
		if (!(BTN_PIN & (1 << BTN_2)) ){
			if(ms - ms2 >= 300){
				sigCount = 2;
				ms2 = ms;
				
				if (ActTimer == 0x00){
					addM(&timerA);
				}
				else{
					addM(&timerB);
				}				
			}
		}
		
		if (!(BTN_PIN & (1 << BTN_3)) ){
			if(ms - ms3 >= 300){
				sigCount = 2;
				
				ms3 = ms;
				if (ActTimer == 0x00){
					ActTimer = 0x01;
				}
				else{
					ActTimer = 0x00;
				}
			}
		}
		
		if (!(BTN_PIN & (1 << BTN_4)) ){
			if(ms - ms4 >= 300){
				sigCount = 2;
				ms4 = ms;
				if (ActTimer == 0x00){
					
					if (StatusA == 0x00){
						StatusA = 0x01;
					}else {
						StatusA = 0x00;
						setNull(&timerA);
					}
				} 
				else{
					if (StatusB == 0x00){
						StatusB = 0x01;
						}else {
						StatusB = 0x00;
						setNull(&timerB);
					}
				}
			}
		}
		
		if(sigCount > 0 & ms - sig >= 500){
			sig = ms;
			Signal();
		}
	}
}

void Signal(){
	SIG_DDR |= (1 << SIG_PIN);
	
	SIG_PORT |= (1 << SIG_PIN);
	
	if(ms - sigU >= 1000){
		sigU = ms;
		SIG_PORT &= ~(1 << SIG_PIN);
		sigCount --;
	}
}

void PrintOnSeg(){
	if (ActTimer == 0x00){
		getTime(&timerA);
		segPrint(&timerA);
	}
	else{
		getTime(&timerB);
		segPrint(&timerB);
	}
};