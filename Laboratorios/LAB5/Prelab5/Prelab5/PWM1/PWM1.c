/*
 * PWM1.c
 *
 * Created: 4/11/2025 4:14:52 PM
 *  Author: juana
 */ 
#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

int DutyCycle(uint8_t lec_ADC){
	return (lec_ADC* 125UL /255UL + 125UL);
}

void initPWM1(){
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1A |= (1<<COM1A1) ; //Modo non-invertido
	
	//Modo FAST PWM con TOP en ICR1
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12)|(1<<WGM13);
	
	TCCR1B |= (1<<CS11);	//Prescaler de 8
	OCR1A=69;
	//OCR1A=300;
	ICR1= 312; //TOP
}