//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Juan Rodríguez
// Carné: 221593
// Proyecto: Laboratorio 5
// Hardware: Atmega238p
// Creado: 11/04/2025
//******************************************************************************

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "PWM1//PWM1.h"
#include "ADC_init/ADC_init.h"

//****Prototipo de funciones****
void setup();
//void init_ADC();


//****Variables globales****
uint8_t  compare=0;
uint8_t	 inv=0;
uint16_t prescaler=8;
uint8_t mode=14;
uint16_t periodo= 39999;
volatile uint8_t valorADC = 0;
volatile uint16_t DUT = 0;

int main()
{
    setup();
    
    while (1) 
    {
		DUT = DutyCycle(valorADC);
		OCR1B = DUT;            // Actualizamos el duty cycle
        _delay_ms(10);  // Pequeño retardo para estabilidad
    }
}

//************Funciones************


void setup(){
    cli();
    //CLKPR = (1<< CLKPCE);
    //CLKPR |= (1<<CLKPS2);    //1Mhz
    
    UCSR0B = 0;                //Comunicación serial
	
	//Puerto C como entrada y pullup deshabilitado.
	DDRC=0x00;
	PORTC=0x00;
	
   // init_ADC();
   ADC_init();
    initPWM1(compare, inv, mode, prescaler, periodo);
    sei();
}

/*
void init_ADC(){
	ADMUX = 0;							
	ADMUX |= (1<<REFS0)|(1<<ADLAR);    // 5V de referencia - Justificación a la izquierda - canal 0
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Interrupciones - Prescaler 128
	ADCSRA |= (1<<ADSC); //Iniciar conversión
}
*/


//************Interrupciones************
ISR(ADC_vect){
    valorADC = ADCH;        // Leemos solo ADCH por justificación izquierda
    ADCSRA |= (1<<ADSC);    // Iniciamos nueva conversión
}