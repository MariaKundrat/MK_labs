#define UBRR 104

#define BufSize 16
#define BufMask (BufSize - 1)
#define CRC_ARRAY_SIZE 256
#define DATA_SIZE 11


unsigned char BufferIN[BufSize], StartBufIN=0, EndBufIN=0;
unsigned char BufferOUT[BufSize], StartBufOUT=0, EndBufOUT=0;

const unsigned char ADDRESS = 27;

unsigned char DATE1[DATA_SIZE] = "03.03.2005";
unsigned char DATE2[DATA_SIZE] = "03.03.2005";
unsigned char DATE3[DATA_SIZE] = "03.03.2005";

unsigned char* DATEs[3];

unsigned char oneTime = 1;

unsigned short crcArray[CRC_ARRAY_SIZE];

void createTable() {
  for (int i = 0; i < CRC_ARRAY_SIZE; i++) {
    unsigned char crc = i;
    for (int j = 0; j < 8; j++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x1D;
      } else {
        crc <<= 1;
      }
    }
    crcArray[i] = crc;
  }
}


void WriteBufOUT(unsigned char value)
{
  BufferOUT[EndBufOUT++] = value;
  EndBufOUT &= BufMask;
  
  UCSR0B |= 1<<UDRIE0;
}

void WriteAllDATEs() {
  for(unsigned char date = 0; date < 3; date++) {
    WriteBufOUT(32);
    _delay_ms(2);
    for(unsigned char symbol = 0; symbol < 10; symbol++) {
      WriteBufOUT(DATEs[date][symbol]);
      _delay_ms(2);
    }
    unsigned short res = CRC8(DATEs[date]);
    unsigned char crcH = (res >> 8);
    unsigned char crcL = res;
    WriteBufOUT(crcH);
    _delay_ms(2);
    WriteBufOUT(crcL);
    _delay_ms(2);
  }
}

unsigned short CRC8(unsigned char* data) {
  unsigned char crc = 0;
  for (int i = 0; i < DATA_SIZE; i++) {
    crc = crcArray[crc ^ data[i]];
  }
  return crc;
}

unsigned char ReadBufIN(void)
{
unsigned char value = BufferIN[StartBufIN++];
StartBufIN &= BufMask;
return value;
}

ISR(USART_RX_vect)
{
  BufferIN[EndBufIN++] = UDR0;
  EndBufIN &= BufMask;
}


ISR(USART_UDRE_vect)
{
  UDR0 = BufferOUT[StartBufOUT++];
  StartBufOUT &= BufMask;                   

  if( StartBufOUT == EndBufOUT )
    UCSR0B &= ~(1<<UDRIE0);

}

void setup() {
  DDRD = 0x04;

  UBRR0H = (unsigned char)(UBRR >> 8);
  UBRR0L = (unsigned char)UBRR;
  
  UCSR0A = 0;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << USBS0);

  DATEs[0] = DATE1;
  DATE2[9] ^= (1 << 6);
  DATEs[1] = DATE2;
  DATE3[6] ^= (1 << 7) | (1 << 0);
  DATEs[2] = DATE3;

  createTable();
}



void loop() {
  if(oneTime) {
    _delay_ms(200);
    PORTD |= 1 << PD2;
    oneTime = 0;
    _delay_ms(2);

    WriteAllDATEs();
    _delay_ms(1);

    PORTD &= ~(1 << PD2);
  }
}
