/*
 * init_TIMER0.c
 *
 * Created: 4/23/2025 10:23:06 AM
 *  Author: juana
 */ 
#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#define NORMAL 0
#define CTC 1

void init_TIMER0(uint16_t prescaler, uint8_t contador, uint8_t mode, uint8_t interrupt){
	TCCR0A=0;
	TCCR0B=0;
	OCR0A=0;
	TCNT0=0;
	TIMSK0=0;
	
	switch (mode){
		case CTC:
		TCCR0A|= (1<<WGM01);
		if (interrupt==1){
			TIMSK0 |= (1<<OCIE0A);
		}
		OCR0A=contador;
		break;
		default: //Normal
		if (interrupt==1){
			TIMSK0 |= (1<<TOIE0);
		}
		TCNT0=contador;
		break;
	}
	
	switch (prescaler){
		case 1: //not prescaling
		TCCR0B |= (1<<CS00);
		break;
		case 8:
		TCCR0B |= (1<<CS01);
		break;
		case 64:
		TCCR0B |= (1<<CS00)|(1<<CS01);
		break;
		case 256:
		TCCR0B |= (1<<CS02);
		break;
		case 1024:
		TCCR0B |= (1<<CS00)|(1<<CS00);
		break;
		default:
		break; //clock stop
	}
	
	
}

uint8_t DutyCycle_LED(uint8_t lec_ADC){
	return (lec_ADC);
}