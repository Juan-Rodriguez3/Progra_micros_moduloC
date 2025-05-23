/*
 * LAB04_ADC.c
 *
 * Created: 24/04/2025
 * Author : Juan Rodriguez
	Laboratorio 6
	comunicaci�n serial UART
 */ 

/*********Librerias*********/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "ADC_init/ADC_init.h"

uint8_t mask_data = 1;
char valor_ADC[30];
char dato;
	
/*********Librerias*********/
void setup(void);
void initUART(void);
void write(char texto);
void writeString(char* texto);

/*********Prototipos de funciones*********/

/*********Prototipos de funciones*********/


/*********main*********/
int main(void)
{
    setup();
    while (1) 
    {
		if (mask_data==1){
			mask_data=0;
			writeString("Seleccione una opci�n:\n");
			writeString("1) Leer POT\n");
			writeString("2) Mostrar Asci\n");
			writeString("\n");
		}
		else if (mask_data==3){
			mask_data=1;	
			write(dato);
			writeString("\n");
			writeString("\n");
		}
		
		
    }
}



/*********Subrutinas NON Interrupts*********/
void setup(void){
	cli();
	initUART();
	ADC_init(1, 5, 0, 1, 128);
	sei();
}

void initUART(void){
	//Configurar los pines PD1 Tx y PD0 Rx
	DDRD=0;
	DDRD |= (1<<PORTD1)|(1<<PORTD7)|(1<<PORTD6);	//PD1 �nica salida
	PORTD= 0;
	UCSR0A = 0;		//No se utiliza doble speed. 
	UCSR0B = 0;		
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);  //Habilitamos interrupciones al recibir, habilitar recepci�n y transmisi�n
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);	//Asincrono, deshabilitado el bit de paridad, un stop bit, 8 bits de datos. 
	UBRR0=103;	//UBBRR0=103; -> 9600 con frecuencia de 16MHz
	
	//Configuraci�n de PORTB
	DDRB =0;
	DDRB |= 0xFF; //PORTB como salida
	PORTB =0;
	
	
}

void write(char texto){
	while ((UCSR0A & (1<<UDRIE0))==0);	//Esperamos a que el registro de datos de USART este vac�o
	UDR0= texto;
}

void writeString(char* texto){
	for(uint8_t i = 0; *(texto+i) !='\0'; i++)
	{
		write(*(texto+i));
	}
	
}




/*********Subrutinas Interrupts*********/
ISR(USART_RX_vect) {
	if (mask_data==2){
		dato=50;
	}
	else{
		dato = UDR0;
	}
	
	
	
	switch (dato){
		case 49:
		writeString(valor_ADC);
		writeString("\n");
		mask_data=1;
		break;
		case 50:
		
		if (mask_data==2){
			dato = UDR0;
			mask_data=3;
		}
		else{
			writeString("Envie un caracter\n");
			mask_data=2;
		}
		
		break;
		default:
		mask_data=1;
		break;
	}
	
	/*
	write(dato);
	mask_data = (dato & 0b00111111);	//Limpiamos los bits mas significativos
	PORTB = mask_data;
	
	mask_data = (dato & 0b11000000);	//dejamos los bits mas significativos
	
	PORTD &= (0b00111111);				//Limpiamos los bits mas significativos del Puerto D
	PORTD |= mask_data;					//Cargamos el nuevo valor
	*/
}

ISR (ADC_vect){
	sprintf(valor_ADC, "La lectura es: %d\n", ADCH);	//Guardo temporalmente el valor del adc como cadena
	ADC_init(1, 5, 0, 1, 128); //releer
}


