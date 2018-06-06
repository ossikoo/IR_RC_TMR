#ifndef GENERIC_H
#define GENERIC_H

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

#define LED PC5
#define F_CPU 14740000L
//#define F_CPU 12000000L
#define LED_ON PORTC |= (1<<PC5);
#define LED_OFF PORTC &= ~(1<<PC5);

#endif
