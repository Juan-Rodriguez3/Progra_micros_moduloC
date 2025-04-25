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
void writeChar(char texto);

/*********Prototipos de funciones*********/

/*********Prototipos de funciones*********/


/*********main*********/
int main(void)
{
    setup();
	writeChar('l');
	writeChar('e');
	writeChar('e');
	writeChar('r');
	writeChar(' ');
	writeChar('e');
	writeChar('l');
	writeChar(' ');
	writeChar('P');
	writeChar('O');
	writeChar('T');
	
	
    while (1) 
    {
		
    }
}
/*********main*********/


/*********Subrutinas NON Interrupts*********/
void setup(void){
	cli();
	initUART();
	sei();
}

void initUART(void){
	//Configurar los pines PD1 Tx y PD0 Rx
	DDRD=0;
	DDRD |= (1<<PORTD1);	//PD1 única salida
	UCSR0A = 0;		//No se utiliza doble speed. 
	UCSR0B = 0;		
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);  //Habilitamos interrupciones al recibir, habilitar recepción y transmisión
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);	//Asincrono, deshabilitado el bit de paridad, un stop bit, 8 bits de datos. 
	UBRR0=103;	//UBBRR0=103; -> 9600 con frecuencia de 16MHz
	
}

void writeChar(char texto){
	while ((UCSR0A & (1<<UDRIE0))==0);	//Esperamos a que el registro de datos de USART este vacío
	UDR0= texto;
}

/*********Subrutinas NON Interrupts*********/


/*********Subrutinas Interrupts*********/

/*********Subrutinas NON Interrupts*********/

