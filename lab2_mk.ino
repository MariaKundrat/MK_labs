void setup() {
   Serial.begin(9600);
   DDRA = 0xFF;
   PORTA = 0x00;

   DDRL = 0x00;
   PORTL = (1 << 3)| (1 << 5);
}

void loop() {
  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == 0xA1) {
      animation();
    } 
    if (inByte == 0xA2) {
      animation2();
    }
  }
  if (!(PINL&(1<<3))) {
    Serial.write(0xB1);
    delay(10);
   }
   if (!(PINL&(1<<5))){
    Serial.write(0xB2);
    delay(10);
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

void animation2() {
  PORTA = 0x80;
  delay(900);
  while (PORTA != 0) {
    PORTA = PORTA >> 2;
    delay(900);
  }
  PORTA = 0x40;
  delay(900);
  while (PORTA != 0) {
    PORTA = PORTA >> 2;
    delay(900);
  }
  PORTA = 0x00;
}
