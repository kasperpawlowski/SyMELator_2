/*
 * config.h
 *
 * Created: 28.06.2017 15:11:05
 *  Author: Kasper
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#define VARIO5_PORT_STEPPER PORTA
#define VARIO5_HALF_BYTE LOW
#define VARIO30_PORT_STEPPER PORTA
#define VARIO30_HALF_BYTE HIGH
#define SPEEDO_PORT_STEPPER PORTB
#define SPEEDO_HALF_BYTE LOW
#define ALTI_PORT_STEPPER PORTB
#define ALTI_HALF_BYTE HIGH
#define ALTI_PORT_SERVO PORTE
#define ALTI_PIN_SERVO 0
#define COMPASS_PORT_STEPPER PORTC
#define COMPASS_HALF_BYTE LOW
#define TURN_PORT_SERVO PORTE
#define TURN_PIN_SERVO 1
#define SLIP_PORT_SERVO PORTE
#define SLIP_PIN_SERVO 2







//*****************************NIE MODYFIKOWAC*****************************//
#define LOW 1
#define HIGH 0

#endif /* CONFIG_H_ */