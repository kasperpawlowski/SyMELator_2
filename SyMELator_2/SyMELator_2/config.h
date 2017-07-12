/*
 * config.h
 *
 * Created: 28.06.2017
 *  Author: Kasper Pawlowski
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

//*****************************NIE MODYFIKOWAC*****************************//
#define VARIO5_PORT_STEPPER A
#define VARIO5_HALF_BYTE LOW_HALF
#define VARIO5_EEPROM_ADDR 0

#define VARIO30_PORT_STEPPER A
#define VARIO30_HALF_BYTE HIGH_HALF
#define VARIO30_EEPROM_ADDR 2

#define SPEEDO_PORT_STEPPER C
#define SPEEDO_HALF_BYTE HIGH_HALF
#define SPEEDO_EEPROM_ADDR 4

#define ALTI_PORT_STEPPER C
#define ALTI_HALF_BYTE LOW_HALF
//#define ALTI_PORT_SERVO E - niezalezne od ustawien w tym miejscu, domyslnie PORTE
#define ALTI_PIN_SERVO 3
#define ALTI_EEPROM_ADDR_STEPPER 6
#define ALTI_EEPROM_ADDR_SERVO 8

#define COMPASS_PORT_STEPPER L
#define COMPASS_HALF_BYTE HIGH_HALF
#define COMPASS_EEPROM_ADDR 10

//#define TURN_PORT_SERVO E - niezalezne od ustawien w tym miejscu, domyslnie PORTE
#define TURN_PIN_SERVO 4
#define TURN_EEPROM_ADDR 12

//#define SLIP_PORT_SERVO E - niezalezne od ustawien w tym miejscu, domyslnie PORTE
#define SLIP_PIN_SERVO 5
#define SLIP_EEPROM_ADDR 14

#define GREEN_LED_PORT B
#define GREEN_LED_PIN 1
#define RED_LED_PORT B
#define RED_LED_PIN 7

#define BAUD_RATE 115200
#define CONNECTION_TIMEOUT 1000 //[ms]

#define SDDR(x) DDR ## x
#define DDR(x) SDDR(x)
#define SPORT(x) PORT ## x
#define PORT(x) SPORT(x)
#define LOW_HALF true
#define HIGH_HALF false

#endif /* CONFIG_H_ */