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

uint16_t DutyCycle(uint8_t lec_ADC){
	return (1010UL + lec_ADC * (4000UL/255));
}

void initPWM1(uint8_t compare, uint8_t inv, uint8_t mode, uint16_t prescaler, uint16_t periodo) {
	TCCR1A = 0;
	TCCR1B = 0;
	DDRB = 0;
	PORTB=0;
	
	//OCR1B
	if (compare==0){
		if (inv==0) {
			TCCR1A |= (1<<COM1B1);	//No invertido
		}
		else {
			TCCR1A |= (1<<COM1B1) | (1<<COM1B0);
		}
	}
	//OCR1A
	else {
		if (inv==0) {
			TCCR1A |= (1<<COM1A1);
		}
		else {
			TCCR1A |= (1<<COM1A1) | (1<<COM1A0);
		}
	}
	
	
	switch (mode)
	{
		case 1:	 //PWM Phase correct 8 bits
		TCCR1A |= (1<<WGM10);						
		break;
		
		case 2:	//PWM Phase correct 9 bits		
		TCCR1A |= (1<<WGM11);
		break;
		
		case 3:	//PWM Phase correct 10 bits
		TCCR1A |= (1<<WGM11) | (1<<WGM10);     
		break;
		
		case 5:	//PWM Fast 8 bits
		TCCR1A |=  (1<<WGM10);
		TCCR1B |= (1<<WGM12);
		break;
		
		case 6: //PWM Fast 9 bits
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM12);
		break;
		
		case 7: //PWM Fast 10 bits
		TCCR1A |= (1<<WGM11) | (1<<WGM10);
		TCCR1B |= (1<<WGM12);
		break;
		
		case 8: //PWM Phase and frecuency correct TOP=ICR1 
		TCCR1B |= (1<<WGM13);
		break;
		
		case 9:	//PWM Phase and frecuency correct TOP=OCR1A 
		TCCR1B |= (1<<WGM13);
		TCCR1A |= (1<<WGM10);
		break;
		
		case 10: //PWM Phase correct TOP=ICR1 
		TCCR1B |= (1<<WGM13);
		TCCR1A |= (1<<WGM11);
		break;
		
		case 11: //PWM Phase correct TOP=OCR1A 
		TCCR1A |= (1<<WGM11) | (1<<WGM10);
		TCCR1B |= (1<<WGM13);
		break;
		
		case 14: //PWM Fast TOP=ICR1
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM12) | (1<<WGM13);
		break;
		
		case 15: //PWM Fast TOP=OCR1A 
		TCCR1A |= (1<<WGM11) | (1<<WGM10);
		TCCR1B |= (1<<WGM12) | (1<<WGM13);
		break;
		
		default:	
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM12) | (1<<WGM13);
		break;
	}
	
	switch (prescaler){
		case 1:
		TCCR1B |= (1<<CS10);
		break;
		case 8:
		TCCR1B |= (1<<CS11);
		break;
		case 64:
		TCCR1B |= (1<<CS10)|(1<<CS11);
		break;
		case 256:
		TCCR1B |= (1<<CS12);
		break;
		case 1024:
		TCCR1B |= (1<<CS10)|(1<<CS12);
		break;
		default:
		TCCR1B &= ~((1<<CS10)|(1<<CS12)|(1<<CS11));
		break;
		}	
	/*
	// Configuraci?n para PWM en OC1B (PB2)
	TCCR1A |= (1 << COM1B1);  // PWM no-invertido
	
	// Modo Fast PWM con TOP en ICR1 (Modo 14)
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	
	// Prescaler de 1024 (Frecuencia PWM = 50Hz)
	TCCR1B |= (1<<CS11);
	//OCR1B =1010;
	//OCR1B =4990;*/
	
	ICR1 = periodo;   // TOP value para 50Hz
	
	DDRB |= (1 << DDB2)|(1 << DDB3)|(1 << DDB1);  // Configura PB2 (OC1B) como salida
}