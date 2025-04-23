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
uint8_t canal_ADC=1;	// =0 --> D10/OC1B - !=0 --> D9/OC1A		
uint16_t prescaler_PWM=8;
uint8_t prescaler_ADC=128;
uint8_t mode_PWM=14;
uint16_t periodo= 39999;


volatile uint8_t valorADC = 0;
volatile uint16_t DUT = 0;

int main()
{
    setup();
    
    while (1) 
    {
		ADC_init(1,5,canal_ADC,1,prescaler_ADC);
        _delay_ms(2);  // Pequeño retardo para estabilidad
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
   initPWM1(0, 0, mode_PWM, prescaler_PWM, periodo);
    sei();
}


//************Interrupciones************
ISR(ADC_vect){
	
	//Actualizamos el valor del Dutty cycle
    valorADC = ADCH;        // Leemos solo ADCH por justificación izquierda
	DUT = DutyCycle(valorADC);
	OCR1B = DUT;            // Actualizamos el duty cycle
	OCR1A = DUT;
	//Actualizamos el comparador/canal del PWM
	
	
	//Iniciamos la conversión 
    ADCSRA |= (1<<ADSC);    // Iniciamos nueva conversión
}