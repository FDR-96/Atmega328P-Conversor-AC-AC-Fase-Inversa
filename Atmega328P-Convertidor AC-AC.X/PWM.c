
/*
 * File:   USART.c
 * Author: ROLDAN
 *

 */

 #include "PWM.h"
 int _freq = 0;
 #define F_CPU 16000000UL
 #include <xc.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <stdbool.h>


void PWM_init(int freq)
{
	//Operation Mode = Fast PWM
	TCCR1A &=~ (1<<WGM10);
	TCCR1A |=  (1<<WGM11);
	TCCR1B |=  (1<<WGM12);
	TCCR1B |=  (1<<WGM13);
	
	//Compare value
	_freq = freq;
	ICR1 = (F_CPU/1024/_freq) - 1;
}

void PWM_on()
{
        TCNT1 = 0x0000;
        //N = 1024
        TCCR1B |=  (1<<CS10);
        TCCR1B &=~ (1<<CS11);
        TCCR1B |=  (1<<CS12);
}
    

void PWM_off()
{
	//Clock setting T1clock = 0 Hz
	TCCR1B &=~ (1<<CS10);
	TCCR1B &=~ (1<<CS11);
	TCCR1B &=~ (1<<CS12);
}

void PWM_setDuty(int duty)
{
	//Pin configuration
	DDRB |= (1<<DDB1);
	
	//Output active
	TCCR1A &=~ (1<<COM1A0);
	TCCR1A |=  (1<<COM1A1);

	OCR1A = (((F_CPU/1024/_freq) - 1)*duty)/100;
}
