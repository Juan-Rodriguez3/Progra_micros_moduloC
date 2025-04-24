//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Juan Rodríguez
// Carné: 221593
// Proyecto: Laboratorio 5
// Hardware: Atmega238p
// Creado: 11/04/2025
//******************************************************************************

//Librerías
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "PWM1//PWM1.h"
#include "ADC_init/ADC_init.h"
#include "TIMER0/init_TIMER0.h"
#define  Vref_5V 5
#define ON 1
#define OFF 0
#define prescaler_PWM 8
#define prescaler_ADC 128
#define mode_PWM 14
#define periodo 39999
#define comparador 2
#define prescaler_TIMER0 8
#define T0_value 95

//****Prototipo de funciones****
void setup();

//****Variables globales****
volatile uint8_t canal_ADC=0;	// =0 --> D10/OC1B - !=0 --> D9/OC1A
volatile uint8_t valorADC = 0;	//Lectura del adc
volatile uint16_t DUT = 0;	//Duty_cycle de los Servos
volatile uint8_t DUT_LED = 0; //Duty cycle del led
volatile uint8_t  contador_led = 0;	//contador del led

int main()
{
    setup();
    while (1)
    {
		if (contador_led<DUT_LED){
			PORTB |= (1<<PORTB3);
		}
		else if (contador_led>=DUT_LED){
			PORTB &= ~(1<<PORTB3);
		}
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
	
	DDRB = 0;
	DDRB |= (1 << DDB3)|(1 << DDB2)|(1 << DDB1); //Configurar el puerto de Led
	PORTB=0;
	
	initPWM1(comparador, OFF, mode_PWM, prescaler_PWM, periodo);
	ADC_init(ON, Vref_5V,canal_ADC,ON,prescaler_ADC);
	//F_cpu 16MHz y prescaler de 8
	init_TIMER0(prescaler_TIMER0, T0_value, OFF, ON);				//Interrupción cada 0.10 ms --> periodo = 0.08 ms *255 = 20 ms
	TCNT0=0;
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
		break;
		case 1:
		OCR1A = DUT;			// Actualizamos el duty cycle
		break;
		case 2:
		DUT_LED=DutyCycle_LED(valorADC);
		break;
		default:
		break;
	}
	
	//Multiplexeo de canales de ADC para la proxuma lectura.
	if (canal_ADC>=2){
		canal_ADC=0;
	}
	else {
		canal_ADC++;
	}
	
	//Reconfiguracion del ADC
	ADC_init(ON, Vref_5V,canal_ADC,ON,prescaler_ADC);
}

ISR(TIMER0_OVF_vect){
	contador_led++;
	if (contador_led==255){
		contador_led=0;	//Reiniciamos el contador.
	}
	TCNT0=T0_value;			
}
