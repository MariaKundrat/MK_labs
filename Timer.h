#ifndef TIMER_H_
#define TIMER_H_

struct Time{
	int h;
	int min;
	int sec;
	int strList[10];
} ;

void initTimer(){
	SREG = 0x00;
	
	DDRC = 0xFF;

	TCCR1A = 0x00;
	TCCR1B |= (1 << CS12);
	TCCR1B |= ((1 << CS10) | (1 << WGM12));
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 0x3D08;
	

	SREG = 0x80;
}

void getTime(struct Time *timest){
	int hc = timest->h;
	int minc = timest->min;
	int secc = timest->sec;
	int i = 0;
	
	while (secc > 0) {
		int digit = secc % 10;
		timest->strList[i] =0x30 + digit;
		secc /= 10;
		i++;
	}
	
	for (i; i < 2; i++){
		timest->strList[i] = 0x30;
	}
	
	timest->strList[i] = 0x3A;	
	i++;
	
	while (minc > 0) {
		int digit = minc % 10;
		timest->strList[i] = 0x30 + digit;
		minc /= 10;
		i++;
	}
	
	for (i; i < 5; i++){
		timest->strList[i] = 0x30;
	}
	
	timest->strList[i] = 0x3A;
	i++;

	while (hc > 0) {
		int digit = hc % 10;
		timest->strList[i] = 0x30 + digit;
		hc /= 10;
		i++;
	}
	
	for (i; i < 8; i++){
		timest->strList[i] = 0x30;
	}
}

char TimerWork(struct Time *timest){
	if(timest->sec > 0){
		timest->sec --;
		return 0x01;
	}
	else if(timest->min > 0){
		timest->min --;
		timest->sec = 59;
		return 0x01;
	}
	else if (timest->h > 0){
		timest->h --;
		timest->min = 59;
		timest->sec = 59;
		return 0x01;
	}
	else{
		return 0x00;
	}
}


void setNull(struct Time *timer){
	timer->h = 0;
	timer->min = 0;
	timer->sec = 0;
}

void setTimer(struct Time *timer ,int hours, int minutes, int seconds){
	if(hours <= 24){
		timer->h = hours;
	}
	else{
		timer->h = 24;
				PORTE |= (1 << 1);
				_delay_ms(200);
				PORTE = 0x00;
	}
	
	
	if (minutes < 60){
		timer->min = minutes;
	}else{
		timer->min = 59;
				PORTE |= (1 << 1);
				_delay_ms(200);
				PORTE = 0x00;
	}
	
	if (seconds < 60){
		timer->sec = seconds;
	}else{
		timer->sec = 59;
				PORTE |= (1 << 1);
				_delay_ms(200);
				PORTE = 0x00;
	}
}



#endif