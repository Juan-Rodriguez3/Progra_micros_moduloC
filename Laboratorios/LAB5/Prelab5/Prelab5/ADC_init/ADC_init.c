/*
 * CFile1.c
 *
 * Created: 4/21/2025 1:14:02 AM
 *  Author: juana
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

void initADC(){
	ADMUX = 0;							//Canal del ADC
	ADMUX |= (1<<REFS0)|(1<<ADLAR)|(1<<MUX1);    // 5V de referencia - Justificación a la izquierda
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Interrupciones - Prescaler 128
	ADCSRA |= (1<<ADSC); //Iniciar conversión
}