#ifndef usart_h
#define usart_h
/* USART interface */

#define FOSC 14740000L

#define BAUD 115200
//#define MYUBRR (FOSC/16/BAUD)-1, this gives 6.9997 rounds to 6 => fails on 115200!
#define MYUBRR 7

void USART_Init( void );
void USART_Transmit( unsigned char data);
void USART_Str(unsigned char *str, int len);

#endif
