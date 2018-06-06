/* USART interface */

#include <avr/io.h>
#include "usart.h"

void USART_Init( void )
{
  unsigned int ubrr = MYUBRR;
  //power reduction off for usart 0
  PRR &=  ~(1<<PRUSART0);

  /*Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  /*enable transmitter */
  UCSR0B = (1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  //UCSR0C = (1<<USBS0)|(3<<UCSZ00);  
  //8n1
  //Usart external clock io?
  //  DDRD &= ~(1<<PD4);
  //  PORTD &= ~(1<<PD4);

  UCSR0C = (3<<UCSZ00); //8n1, async
  //UCSR0C &= ~((1<<UPM00)|(1<<UPM01)); //n parity
  //UCSR0C &= ~((1<<USBS0)); //1 stop
  UCSR0C &= ~((1<<UMSEL00) | (1<<UMSEL01)); //unsync op
  //set buffer empty flag
  UCSR0A &= ~((1<<UDRE0) | (1<<U2X0));
}

void USART_Transmit( unsigned char data )
{
  if (data == '\n')
    USART_Transmit('\r');
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)) )
    ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

void USART_Str(unsigned char *str, int len)
{
  int i;
  for(i = 0; i < len; i++)
    USART_Transmit(str[i]);
}

