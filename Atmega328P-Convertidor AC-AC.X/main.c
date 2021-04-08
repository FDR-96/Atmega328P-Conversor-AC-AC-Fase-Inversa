/*
 * 
 * File:   main.c
 * Author: FDPR
 *
 * Created on March 12, 2021, 4:26 PM
 */


#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "PWM.h"
#include "ADC.h"
#include "USARTAtmega328P.h"
#include "WatchDog.h"
#include "PCINT.h"
#define F_CPU 16000000
int _Contador = 0;
unsigned char dutyCharArray[2];
unsigned int duty, unidad, decena, centena;
unsigned char Tx[2];
unsigned char dato = 'R';
int adcV;

ISR (PCINT0_vect) {
      PWM_Update();
      PORTB = (1<<PB2);
}
void Conversion_Char_Int(){
    //Otenemos la centena decena y unidad del array char que se genero al recibir los caracteres
    //de la comunicacion serial
    centena = dutyCharArray[0] * 100;
    decena = dutyCharArray[1] * 10;
    unidad = dutyCharArray[2];
    //Sumamos los valores enteros para obtener el ancho de pulso que se generara en nuestro PWM
    duty = centena + decena + unidad;  
}
void Desmenuzado_ADC(){
    //Obtenemos la centena, decena y unidad, para luego mandarlos por separados en un array char
    centena = adcV/100;
    decena = (adcV - centena)/10;
    unidad = adcV - centena - decena;
    Tx[0] = centena + 48;
    Tx[1] = decena + 48;        
    Tx[2] = centena + 48;
   
}

void main(void) {
    cli();  

    DDRB = 0b11111100;
    PWM_init(1600);
    USART_init();
    WATCHDOG_init();
    PCINT_init();
    PWM_setDuty(70);
    PWM_on();
    sei(); 

    for(;;){
      USART_SetData(dato);
        switch(USART_GetData()){
            case '#':
                while(PINB != 0b1000000);
                PWM_setDuty(90);
                PWM_on();
                break;
            case '%':
                while(_Contador <= 3){
                    dutyCharArray[_Contador] = USART_GetData();
                    _Contador++;
                }
                _Contador = 0;
                Conversion_Char_Int();
                PWM_setDuty(duty);
                PWM_on();
                break;
            case 'V':
                ADC_GetData(0);
                adcV = ADC_GetData(0)*5/1024;
                Desmenuzado_ADC();
                USART_SetData('V');
                while(_Contador <= 2){
                    USART_SetData(Tx[_Contador]);
                    _Contador++;
                }
                _Contador = 0;
                break;
        }
       
    }
}
