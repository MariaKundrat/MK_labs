#ifndef TIMER_H_
#define TIMER_H_

struct Time{
	int h;
	int min;
	int sec;
	int strList[6];
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


int segGet(int dig){
	int i = 0;
	int curr = 0x00;
	while(i != dig){
		i++;
	}
	curr = ~(arr[i]);
	return curr;
}


void getTime(struct Time *timest){
	int hc = timest->h;
	int minc = timest->min;
	int secc = timest->sec;
	int i = 0;
	
	while (secc > 0) {
		int digit = secc % 10;
		timest->strList[i] = segGet(digit);
		secc /= 10;
		i++;
	}
	
	for (i; i < 2; i++){
		timest->strList[i] = segGet(0);
	}
	
	while (minc > 0) {
		int digit = minc % 10;
		timest->strList[i] = segGet(digit);
		minc /= 10;
		i++;
	}
	
	for (i; i < 4; i++){
		timest->strList[i] = segGet(0);
	}

	while (hc > 0) {
		int digit = hc % 10;
		timest->strList[i] = segGet(digit);
		hc /= 10;
		i++;
	}
	
	for (i; i < 6; i++){
		timest->strList[i] = segGet(0);
	}
	
	timest->strList[2] |= (1 << DP);
	timest->strList[4] |= (1 << DP);
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

void addH(struct Time *timer){
	if (timer->h < 24){
		timer->h ++;
	}
	else{
		timer->h = 0;
	}
}

void addM(struct Time *timer){
	if (timer->min < 60){
		timer->min ++;
	} 
	else{
		timer->min = 0;
	}
}

void segPrint(struct Time * timer){
	
	for (int Pos = 0; Pos < 6; Pos++){
		int posLn = LNL[Pos];
		
		V_PORT = (1 <<posLn);
		SEG_PORT = 	timer->strList[Pos];
		if (Pos%2 == 0 & Pos != 0){
			SEG_PORT &= ~(1 << DP);
		}
		_delay_ms(10);
	}
}


#endif