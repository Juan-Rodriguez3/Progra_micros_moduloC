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

//Prototipos
void setup();


//Funcion principal
int main(void)
{
    setup();
    while (1) 
    {
		PORTB = (1<<PORTB2);
		switch (pb){
			case 1:
			PORTD++;			//Incrementar contador
			pb=0;				//Resetear pb
			break;
			
			case 2:
			PORTD--;			//Decrementar contador
			pb=0;				//Resetear pb
			break;		
					
			default:
			break;
		}
    }
}

//Subrutinas NON Interrupt
void setup () {
	
	
	
	DDRC = 0x00;		//Pines PC0, PC1, PC2 como entrada
	PORTC |= (1<<PORTC0) | (1<<PORTC1) ;		//pullups portc
	
	DDRD = 0xFF;		//Puerto D como salida
	PORTD= 0x00;
	
	DDRB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2);
	PORTB = 0x00;		//Pines PB0, PB1 y PB2 como salida
	
	PCICR |= (1 << PCIE1);		//habilita interrupciones en el PORTC
	PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);	//interrupciones para PC0 y PC1
	
	sei();
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