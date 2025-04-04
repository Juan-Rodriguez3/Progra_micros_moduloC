/*
 * Lab4.c
 * Author: Juan Rodriguez
 * Description: Prelab4
 */
//Librerias y variables
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
uint8_t pb=0;			//Estados de botones
uint8_t contador=0;		//Contador para display
uint8_t top=0xFF;
uint8_t bottom=0;

//Prototipos
void setup();
int OVF_UNF(uint8_t cont,  uint8_t top, uint8_t bottom);

//Funcion principal
int main(void)
{
    setup();
    while (1) 
    {
		PORTB = (1<<PORTB2);
		contador= OVF_UNF(contador, top, bottom);
		PORTD= contador;
    }
}

//Subrutinas NON Interrupt
void setup () {
	cli();				//Desactivar interrupciones globales
	
	DDRC = 0x00;		//Pines PC0, PC1, PC2 como entrada
	PORTC |= (1<<PORTC0) | (1<<PORTC1) ;		//pullups portc
	
	DDRD = 0xFF;		//Puerto D como salida
	PORTD= 0x00;
	
	DDRB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2);
	PORTB = 0x00;		//Pines PB0, PB1 y PB2 como salida
	
	PCICR |= (1 << PCIE1);		//habilita interrupciones en el PORTC
	PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);	//interrupciones para PC0 y PC1
	
	sei();			//Activar interrupciones globales
}

int OVF_UNF(uint8_t cont, uint8_t top, uint8_t bottom){
	
	if (cont==bottom && pb==2){
		cont=top;
		pb=0;
		return cont;
	}
	else if (cont==top && pb==1){
		cont=bottom;
		pb=0;
		return cont;
	}	
	else if(pb==1){
		cont++;
		pb=0;
		return cont;
	}
	else if( pb==2){
		cont--;
		pb=0;
		return cont;
	}
	else {
		return cont;
	}
}

//Subrutinas NON Interrupt	


//Subrutinas de interrupciones
ISR(PCINT1_vect){
	
	if (!(PINC & (1<<PINC0))){
		pb=1;
	}
	else if (!(PINC & (1<<PINC1))){
		pb=2;
	}
	else{
		pb=0;
	}
}