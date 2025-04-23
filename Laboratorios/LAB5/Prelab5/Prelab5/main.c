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
uint16_t prescaler_PWM=8;
uint8_t prescaler_ADC=128;
uint8_t mode_PWM=14;
uint16_t periodo= 39999;

volatile uint8_t canal_ADC=0;	// =0 --> D10/OC1B - !=0 --> D9/OC1A
volatile uint8_t valorADC = 0;
volatile uint16_t DUT = 0;

int main()
{
    setup();
    
    while (1) 
    {
       _delay_ms(1);  // Pequeño retardo para estabilidad	
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
	initPWM1(2, 0, mode_PWM, prescaler_PWM, periodo);
	ADC_init(1,5,canal_ADC,1,prescaler_ADC);

    sei();
}


//************Interrupciones************
ISR(ADC_vect){
	
	//Actualizamos el valor del Dutty cycle
    valorADC = ADCH;        // Leemos solo ADCH por justificación izquierda
	DUT = DutyCycle(valorADC);
	switch(canal_ADC){
		case 0:
		OCR1B = DUT;            // Actualizamos el duty cycle
		//OCR1A = DUT;      //Para probar si servía el configurar ambas salidas de PWM
		break;
		case 1:
		OCR1A = DUT;			// Actualizamos el duty cycle
		break;
		default:
		break;
	}
	
	//Multiplexeo de canales de ADC para la proxuma lectura.
	if (canal_ADC>=1){
		canal_ADC=0;
	}
	else {
		canal_ADC++;
	}
	
	//Reconfiguracion del ADC
	ADC_init(1,5,canal_ADC,1,prescaler_ADC);
}