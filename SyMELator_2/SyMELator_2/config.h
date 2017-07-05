/*
 * config.h
 *
 * Created: 28.06.2017
 *  Author: Kasper Pawlowski
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

//*****************************NIE MODYFIKOWAC*****************************//
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

#define GREEN_LED_PORT PORTD
#define GREEN_LED_PIN 0
#define RED_LED_PORT PORTD
#define RED_LED_PIN 1

#define LOW true
#define HIGH false
#define BAUD_RATE 9600
#define CONNECTION_TIMEOUT 1000 //[ms]

#endif /* CONFIG_H_ */