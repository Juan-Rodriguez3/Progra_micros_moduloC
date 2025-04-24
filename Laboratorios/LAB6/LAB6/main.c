/*
 * LAB04_ADC.c
 *
 * Created: 24/04/2025
 * Author : Juan Rodriguez
	Laboratorio 6
	comunicación serial UART
 */ 

/*********Librerias*********/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
	
/*********Librerias*********/
void setup(void);
void initUART(void);

/*********Prototipos de funciones*********/

/*********Prototipos de funciones*********/


/*********main*********/
int main(void)
{
    setup(void);
    while (1) 
    {
		
    }
}
/*********main*********/


/*********Subrutinas NON Interrupts*********/
void setup(void){
	cli();
	initUART(void);
	sei();
}

void initUART(void){
	//Configurar los pines PD1 Tx y PD0 Rx
	DDRD=0;
	DDRD |= (1<<PORTD1);	//PD1 única salida
	UCSR0A = 0;
	UCSR0B = 0;
	UCSR0C = 0;
	
}

/*********Subrutinas NON Interrupts*********/


/*********Subrutinas Interrupts*********/

/*********Subrutinas NON Interrupts*********/

