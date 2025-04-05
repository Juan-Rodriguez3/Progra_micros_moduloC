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
uint8_t DIPSL=0;
uint8_t DIPSH=0;
uint8_t cont_hex=0;
uint8_t multiplex=1;
uint8_t delay_ms=208;

uint8_t DISPLAY[16] = {0xF3, 0x81, 0xEA, 0xE9, 0x99, 0x79, 0x7B, 0xC1, 0xFB, 0xF9, 0xDB, 0x3B, 0x72, 0xAB, 0x7A, 0x5A};

//Prototipos
void setup();
void initADC();
void MUX(uint8_t flag);
void initTIMER0();
int OVF_UNF(uint8_t cont,  uint8_t top, uint8_t bottom);

//Funcion principal
int main(void)
{
    setup();
    while (1) 
    {
		MUX(multiplex);
		if (contador==ADCH){
			PORTB |= (1<<PORTB3);		//Encender Led de alarma
		}
		else{
			PORTB &= ~(1<<PORTB3);
		}
		
    }
}

//Subrutinas NON Interrupt
void setup () {
	cli();				//Desactivar interrupciones globales
	
	CLKPR = (1<< CLKPCE);
	CLKPR |= (1<<CLKPS2);	//Configurar prescaler principal a 16
	
	DDRC = 0x00;		//Pines PC0, PC1, PC2 como entrada
	PORTC |= (1<<PORTC0) | (1<<PORTC1) ;		//pullups PC0,PC1
	PORTC &= ~(1 << PC2);						//Deshabilitar el Pullup PC2	
	
	DDRD = 0xFF;		//Puerto D como salida
	PORTD= 0x00;
	
	DDRB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2)| (1 << PORTB3);
	PORTB = 0x00;		//Pines PB0, PB1 y PB2 como salida
	
	PCICR |= (1 << PCIE1);		//habilita interrupciones en el PORTC
	PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);	//interrupciones para PC0 y PC1
	
	initADC();
	initTIMER0();
	UCSR0B = 0x00;  // Desactiva transmisor y receptor
	  
	sei();			//Activar interrupciones globales
}

void initTIMER0(){
	TCCR0A=0;
	TCCR0B |= (1<<CS01)|(1<<CS00);			//Modo normal prescaler 8
	TCNT0= delay_ms;
	TIMSK0|=(1<<TOIE0);			//Habilitar interrupciones de desbordamiento.
}



void initADC(){
	ADMUX=0;
	ADMUX |= (1<<MUX1);			//Selecionar el canal 2
	ADMUX |= (1<<ADLAR);		//Justificaci?n a la derecha
	ADMUX |= (1<<REFS0);		//Voltaje de referencia 5V
	
	ADCSRA=0;
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS1) |(1<<ADPS0) ; // Habilitar ADC, interrupciones ADC y prescaler de 8
	ADCSRA |= (1<<ADSC);		// Empieza a leer o a hacer la conversi?n
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

void MUX(uint8_t flag){
	switch (flag){
		case 0:
		contador= OVF_UNF(contador, top, bottom);
		PORTD= contador;
		PORTB = 0b00000100;
		break;
		case 1:
		cont_hex=DISPLAY[DIPSH];  //Mostrar parte alta 
		PORTD = cont_hex;
		PORTB = 0b00000001;
		break;
		case 2:
		cont_hex=DISPLAY[DIPSL];	//Mostrar parte baja
		PORTD= cont_hex;
		PORTB = 0b00000010;
		break;
		default:
		break;
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
ISR(ADC_vect){
	DIPSL= (ADCH & 0x0F);		//Parte baja de la lectura de ADC
	DIPSH= (ADCH>>4) ;		//Parte alta desplaza a los bits menos significativos para que su valor sea entre 0 a 15
	ADCSRA |= (1<<ADSC);		//Vuelve a leer
}



ISR(TIMER0_OVF_vect){
	TCNT0= delay_ms;				//Reiniciar el contador
	multiplex++;
	if (multiplex==3){
		multiplex=0;
	}
	
}