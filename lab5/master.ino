#define UBRR 104

#define BufSize 32
#define BufMask (BufSize - 1)
unsigned char BufferOUT[BufSize], StartBufOUT=0, EndBufOUT=0;


void WriteBufOUT(unsigned char value) 
{
  BufferOUT[EndBufOUT++] = value;
  EndBufOUT &= BufMask;         
  UCSR1B |= 1<<UDRIE1;             
}

ISR(USART1_RX_vect)
{
  UDR0 = UDR1;
}

ISR(USART1_UDRE_vect )             
{
  UDR1 = BufferOUT[StartBufOUT++];
  StartBufOUT &= BufMask;                   

  if( StartBufOUT == EndBufOUT )             
    UCSR1B &= ~(1<<UDRIE1);                  

}

void setup() {
  DDRD = 0x02;

  UBRR1H = (unsigned char)(UBRR >> 8);
  UBRR1L = (unsigned char)UBRR;
  
  UCSR1A = 0;
  UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10) | (1 << USBS1);

  UBRR0H = (unsigned char)(UBRR >> 8);
  UBRR0L = (unsigned char)UBRR;
  
  UCSR0A = 0;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << USBS0);
}

void loop() {
  // if(!(PINC & (1 << PINC1))) {
  //   PORTD |= (1 << PD1);
  //   _delay_ms(1);
  //   WriteBufOUT(28);
  //   _delay_ms(1);
  //   PORTD &= ~(1 << PD1);
  // }
  // if(!(PINC & (1 << PINC2))) {
  //   PORTD |= (1 << PD1);
  //   _delay_ms(1);
  //   WriteBufOUT(27);
  //   _delay_ms(1);
  //   PORTD &= ~(1 << PD1);
  // }
}
