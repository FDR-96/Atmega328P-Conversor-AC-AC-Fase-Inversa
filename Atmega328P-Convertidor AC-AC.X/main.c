/*
 * 
 * File:   main.c
 * Author: FDPR
 *
 * Created on March 12, 2021, 4:26 PM
 */
#define F_CPU 16000000

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "PWM.h"
#include "ADC.h"
#include "USARTAtmega328P.h"
#include "WatchDog.h"
#include "PCINT.h"
#include <stdbool.h>
#include <util/delay.h>

int _Contador = 0;
int contChar = 1;
unsigned char dutyCharArray[2];
unsigned int duty = 0;
float unidad = 0;
float decena = 0;
float centena = 0;
unsigned char Tx[2];
unsigned char dato = 'R';
unsigned char Serial[5];
float adcV;
int val = 0;
bool update = false;

char comoCaracter;

void Conversion_Char_Int(){
    //Otenemos la centena decena y unidad del array char que se genero al recibir los caracteres
    //de la comunicacion serial
    decena =  dutyCharArray[0] - '0';
    decena = decena  * 10;
     unidad =  dutyCharArray[1] - '0';
    unidad =  unidad * 1;
    //Sumamos los valores enteros para obtener el ancho de pulso que se generara en nuestro PWM
    duty =  decena + unidad;  
}
void Desmenuzado_ADC(){
    //Obtenemos la centena, decena y unidad, para luego mandarlos por separados en un array char
   adcV = adcV*100;
    centena = adcV/100;
    decena = (adcV - centena)/10;
    unidad = adcV - centena - decena;
    Tx[0] = centena + '0' ;
    Tx[1] = decena + '0';        
    Tx[2] = unidad + '0';
    
}

void main(void) {
    cli();  
    DDRB |= (1<<DDB1)||(1<<DDB2)||(1<<DDB5);
    PORTB |= 0b10000000;
 
    PWM_init(100);
    USART_init();
  
    PCINT_init();
    sei(); 

    
    while(1){
        //Encender
        //Apagar
        //Disparo:
        //Voltaje
        Serial[0] = USART_GetData();
        USART_SetData(Serial[0]);
        switch(Serial[0]){
            case 'A':
                while(contChar <= 5){
                    Serial[contChar] = USART_GetData();
                    USART_SetData(Serial[contChar]);
                    contChar++;     
                }
                contChar = 1;
                break;
            case 'E':
                while(contChar <= 8){
                    Serial[contChar] = USART_GetData();
                    USART_SetData(Serial[contChar]);
                    contChar++;     
                }
                contChar = 1;
                break;
                
            case 'D':
                while(contChar <= 7){
                    Serial[contChar] = USART_GetData();
                    USART_SetData(Serial[contChar]);
                    contChar++;     
                }
                contChar = 1;
                break;
                
            case 'V':
                while(contChar <= 7){
                    Serial[contChar] = USART_GetData();
                    USART_SetData(Serial[contChar]);
                    contChar++;     
                }
                contChar = 1;
                break;
                
            default:
                break;
        }
        

        
        switch(Serial[0]){
            case 'E':
                PWM_setDuty(20);
                  USART_SetData(dato);
                  update = true;
              
                   break;
            case 'A':
                PWM_off();
                   break;
            case 'D':
                
                while(_Contador <= 2){
                    dutyCharArray[_Contador] = USART_GetData();
                    USART_SetData(dutyCharArray[_Contador]);
                    _Contador++;
                }
                    _Contador = 0;
                Conversion_Char_Int();
                PWM_setDuty(duty);
                duty = 0;
                PORTB |= (1<<PORTB5);
             
               
                break;
            case 'V':
                adcV = ADC_GetData(0)*5.0f/1024.0f;
                Desmenuzado_ADC();
                USART_SetData('V');
                USART_SetData(':');
                USART_SetData(Tx[0]);
                USART_SetData(Tx[1]);
                USART_SetData(Tx[2]);
                
                USART_GetData();
                _Contador++;   
            
                _Contador = 0;
                break;
        }
       
    }
    
}

ISR(INT0_vect){
 
  
    if(update){
    PWM_off();
    PWM_on();
    update = false;
    }
    
}

