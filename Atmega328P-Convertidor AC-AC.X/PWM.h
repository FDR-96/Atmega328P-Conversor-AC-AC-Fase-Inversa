/*
 * File:   USART.c
 * Author: ROLDAN
 *

 */


#ifndef PWM_H_
#define PWM_H_

	#include <avr/io.h>

	void PWM_init(int freq);
	void PWM_on();
	void PWM_off();
    void PWM_Update();
	void PWM_setDuty(int duty);
	

#endif 