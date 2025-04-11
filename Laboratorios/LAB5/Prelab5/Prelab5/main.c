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

//****Prototipo de funciones****
void setup();
void initADC();
void initPWM1();
int DutyCycle(uint8_t lec_ADC);

uint8_t valorADC=0;

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
	
	DDRB = (1<<PORTB1) | (1<<PORTB2)| (1<<PORTB3);		//PB1 - PB3 como salidas
	PORTB= 0x00;		//conduce 0 lógico
	
	UCSR0B = 0;
	initADC();
	initPWM1();
	sei();
}

void initADC(){
	ADMUX = 0;
	ADMUX |= (1<<REFS0)|(1<<ADLAR);	// 5V de referencia - Justificación a la izquierda
	
	ADCSRA = 0;
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS1)|(1<<ADPS0); //Habilitación de ADC y interrupción - Prescaler para 125 Khz 
}

void initPWM1(){
								
	TCCR1A = 0;
	TCCR1B = 0;
	
	TCCR1A |= (1<<COM1A1) ; //Modo non-invertido
	//Modo FAST PWM con TOP en ICR1
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12)|(1<<WGM13);
	TCCR1B |= (1<<CS11);	//Prescaler de 8
	
	ICR1= 6553; //TOP
}

int DutyCycle(uint8_t lec_ADC){
	
	return lec_ADC;
}

//************Interrupciones************
ISR(ADC_vect){
	valorADC = ADCH;
	ADCSRA |= (1<<ADSC);
}