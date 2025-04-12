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

//****Prototipo de funciones****
void setup();
void initADC();
uint16_t DutyCycle(uint8_t lec_ADC);

//****Variables globales****
volatile uint8_t valorADC = 0;
volatile uint16_t DUT = 0;

int main()
{
    setup();
    
    while (1) 
    {
        _delay_ms(10);  // Pequeño retardo para estabilidad
    }
}

//************Funciones************
void setup(){
    cli();
    //CLKPR = (1<< CLKPCE);
    //CLKPR |= (1<<CLKPS2);    //1Mhz
    
    UCSR0B = 0;                //Comunicación serial
    initADC();
    initPWM1();
    sei();
}

void initADC(){
    ADMUX = 1;
    ADMUX |= (1<<REFS0)|(1<<ADLAR);    // 5V de referencia - Justificación a la izquierda
    
    ADCSRA = 0;
    ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescaler 128
    ADCSRA |= (1<<ADSC); //Iniciar conversión
}



//************Interrupciones************
ISR(ADC_vect){
    valorADC = ADCH;        // Leemos solo ADCH por justificación izquierda
    DUT = DutyCycle(valorADC);
    OCR1B = DUT;            // Actualizamos el duty cycle
    ADCSRA |= (1<<ADSC);    // Iniciamos nueva conversión
}