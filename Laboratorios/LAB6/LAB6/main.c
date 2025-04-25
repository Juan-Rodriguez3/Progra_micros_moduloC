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

uint8_t mask_data = 0;
	
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
	write('l');
	write('e');
	write('e');
	write('r');
	write(' ');
	write('e');
	write('l');
	write(' ');
	write('P');
	write('O');
	write('T');
	writeString(" Se me quemo el nano\n");
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
	DDRD |= (1<<PORTD1)|(1<<PORTD7)|(1<<PORTD6);	//PD1 única salida
	PORTD= 0;
	UCSR0A = 0;		//No se utiliza doble speed. 
	UCSR0B = 0;		
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);  //Habilitamos interrupciones al recibir, habilitar recepción y transmisión
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01);	//Asincrono, deshabilitado el bit de paridad, un stop bit, 8 bits de datos. 
	UBRR0=103;	//UBBRR0=103; -> 9600 con frecuencia de 16MHz
	
	//Configuración de PORTB
	DDRB =0;
	DDRB |= 0xFF; //PORTB como salida
	PORTB =0;
	
	
}

void write(char texto){
	while ((UCSR0A & (1<<UDRIE0))==0);	//Esperamos a que el registro de datos de USART este vacío
	UDR0= texto;
}

void writeString(char* texto){
	for(uint8_t i = 0; *(texto+i) !='\0'; i++)
	{
		write(*(texto+i));
	}
	
}

/*********Subrutinas NON Interrupts*********/


/*********Subrutinas Interrupts*********/
ISR(USART_RX_vect) {
	char dato = UDR0;
	write(dato);
	mask_data = (dato & 0b00111111);	//Limpiamos los bits mas significativos
	PORTB = mask_data;
	
	mask_data = (dato & 0b11000000);	//dejamos los bits mas significativos
	
	PORTD &= (0b00111111);				//Limpiamos los bits mas significativos del Puerto D
	PORTD |= mask_data;					//Cargamos el nuevo valor
	
}
/*********Subrutinas NON Interrupts*********/

