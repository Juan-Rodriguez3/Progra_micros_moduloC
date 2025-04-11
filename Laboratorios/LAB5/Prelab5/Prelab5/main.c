//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Juan Rodríguez
// Carné: 221593
// Proyecto: Laboratorio 5
// Hardware: Atmega238p
// Creado: 11/04/2025
//******************************************************************************


#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "PWM1/PWM1.h"

//****Prototipo de funciones****
void setup();
void initADC();


uint8_t valorADC=0;
uint16_t DUT;

int main()
{
    setup();
	
    while (1) 
    {
		
    }
}
//************Funciones************
void setup(){
	cli();
	CLKPR = (1<< CLKPCE);
	CLKPR |= (1<<CLKPS2);	//1Mhz
	DDRB |= (1 << PORTB1);	//PB1 como salida
		
	
	UCSR0B = 0;				//Comunicación serial
	initADC();
	initPWM1();
	sei();
}

void initADC(){
	ADMUX = 0;
	ADMUX |= (1<<REFS0)|(1<<ADLAR);	// 5V de referencia - Justificación a la izquierda
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS1)|(1<<ADPS0); //Habilitación de ADC y interrupción - Prescaler para 125 Khz 
	ADCSRA |= (1<<ADSC); //Iniciar conversión
}




//************Interrupciones************

ISR(ADC_vect){
	valorADC = ADCH;
	DUT=DutyCycle(valorADC);
	//OCR1A=DUT;
	ADCSRA |= (1<<ADSC);
}
