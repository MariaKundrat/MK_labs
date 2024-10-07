void setup() {
  DDRA=0xFF;
  PORTA=0x00;
  
  DDRL=0x00;
  PORTL=0x02;
}

void loop() {
  if (!(PINL&(1<<1))) {
    animation();
  }
}

void animation() {
  int left_p=0x01;
  int right_p=0x80;
  while(left_p<right_p){
    delay(900);
    PORTA=left_p+right_p;
    left_p=left_p<<1;
    right_p=right_p>>1;
  }
  delay(900);
  PORTA=0x00;
}
