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
	return (1000UL + lec_ADC * (4000UL /255));
}

void initPWM1(uint8_t compare, uint8_t inv, uint8_t mode, uint8_t prescaler) {
	TCCR1A = 0;
	TCCR1B = 0;
	
	switch  (compare){
		case 0:
		if (~inv) {
			TCCR1A |= (1<<COM1B1)
		}
		else {
			TCCR1A |= (1<<COM1B1) | (1<<COM1B0)
		}
		break;
		
		//Por default se usara el OCR1A
		default:
		if (~inv) {
			TCCR1A |= (1<<COM1A1)
		}
		else {
			TCCR1A |= (1<<COM1A1) | (1<<COM1A0)
		}
		break;
	}
	// Configuración para PWM en OC1B (PB2)
	TCCR1A |= (1 << COM1B1);  // PWM no-invertido
	
	// Modo Fast PWM con TOP en ICR1 (Modo 14)
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	
	// Prescaler de 8 (Frecuencia PWM = 50Hz)
	TCCR1B |= (1 << CS11);
	//OCR1B =1005;
	//OCR1B =4995;
	ICR1 = 39999;   // TOP value para 50Hz
	
	DDRB |= (1 << DDB2);  // Configura PB2 (OC1B) como salida
}