/*
 * File:   PCINT.c
 * Author: FDPR
 *
 * Created on 25 de marzo de 2021, 05:29
 */


#include <xc.h>

void PCINT_init()
{
  
    DDRD &=~ (1<<DDD2);		// Pin 2 del puerto D como entrada
	PORTD |= (0<<PORTD2);	// pull-up activado

	EICRA &=~ (1<<ISC00);	// INT0 configurado = Flanco de bajada
	EICRA |= (1<<ISC01);

	EIMSK |= (1<<INT0);		// INT0 activado

}