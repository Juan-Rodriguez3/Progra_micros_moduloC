/*
 * init_TIMER0.h
 *
 * Created: 4/23/2025 10:22:26 AM
 *  Author: juana
 */ 


#ifndef INIT_TIMER0_H_
#define INIT_TIMER0_H_


void init_TIMER0(uint16_t prescaler, uint8_t contador, uint8_t mode, uint8_t interrupt);
uint8_t DutyCycle_LED(uint8_t lec_ADC);

#endif /* INIT_TIMER0_H_ */