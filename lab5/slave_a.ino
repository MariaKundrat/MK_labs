#define UBRR 104

#define BufSize 32
#define BufMask (BufSize - 1)
#define CRC_ARRAY_SIZE 256
#define DATA_SIZE 25

unsigned char BufferIN[BufSize], StartBufIN = 0, EndBufIN = 0;
unsigned char BufferOUT[BufSize], StartBufOUT = 0, EndBufOUT = 0;

const unsigned char ADDRESS = 28;

unsigned char PIB1[DATA_SIZE] = "Kundrat Maria Stepanivna";
unsigned char PIB2[DATA_SIZE] = "Kundrat Maria Stepanivna";
unsigned char PIB3[DATA_SIZE] = "Kundrat Maria Stepanivna";

unsigned char* PIBs[3];

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

  UCSR0B |= 1 << UDRIE0;
}

void WriteAllPIBs() {
  for (unsigned char pib = 0; pib < 3; pib++) {
    WriteBufOUT(66);
    _delay_ms(2);
    for (unsigned char symbol = 0; symbol < 25; symbol++) {
      WriteBufOUT(PIBs[pib][symbol]);
      _delay_ms(2);
    }
    unsigned short res = CRC8(PIBs[pib]);
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

ISR(USART_RX_vect) {
  BufferIN[EndBufIN++] = UDR0;
  EndBufIN &= BufMask;
}

ISR(USART_UDRE_vect)
{
  UDR0 = BufferOUT[StartBufOUT++];
  StartBufOUT &= BufMask;

  if (StartBufOUT == EndBufOUT) 
    UCSR0B &= ~(1 << UDRIE0);  
}

void setup() {
  DDRD = 0x04;

  UBRR0H = (unsigned char)(UBRR >> 8);
  UBRR0L = (unsigned char)UBRR;

  UCSR0A = 0;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << USBS0);

  PIBs[0] = PIB1;
  PIB2[0] ^= (1 << 6);  
  PIBs[1] = PIB2;
  PIB3[6] ^= (1 << 4) | (1 << 2) | (1 << 1); 
  PIBs[2] = PIB3;

  createTable();  
}

void loop() {
  if (oneTime) {
    _delay_ms(10);
    PORTD |= 1 << PD2;  
    oneTime = 0;
    _delay_ms(2);

    WriteAllPIBs();
    _delay_ms(1);

    PORTD &= ~(1 << PD2);  
  }
}
