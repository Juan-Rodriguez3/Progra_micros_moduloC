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
void initADC();


//****Variables globales****
volatile uint8_t  compare=0;
volatile uint8_t	 inv=0;
volatile uint16_t prescaler=8;
volatile uint8_t modo=14;
volatile uint16_t periodo= 39999;
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
	
    initADC();
    initPWM1(compare, inv, modo, prescaler, periodo);
    sei();
}

void init_ADC(){
	ADMUX = 0;							//Canal del ADC
	ADMUX |= (1<<REFS0)|(1<<ADLAR)|(1<<MUX1);    // 5V de referencia - Justificación a la izquierda
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Interrupciones - Prescaler 128
	ADCSRA |= (1<<ADSC); //Iniciar conversión
}

//************Interrupciones************
ISR(ADC_vect){
    valorADC = ADCH;        // Leemos solo ADCH por justificación izquierda
    ADCSRA |= (1<<ADSC);    // Iniciamos nueva conversión
}