/*
 * config.h
 *
 * Created: 28.06.2017
 *  Author: Kasper Pawlowski
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

//*****************************NIE MODYFIKOWAC*****************************//
#define TEST 1
//****************************USTAWIENIA PORTÓW****************************//
#define VARIO5_PORT_STEPPER A
#define VARIO5_HALF_BYTE LOW_HALF
#define VARIO5_EEPROM_ADDR 0

#define VARIO30_PORT_STEPPER A
#define VARIO30_HALF_BYTE HIGH_HALF
#define VARIO30_EEPROM_ADDR 2

#define SPEED_PORT_STEPPER C
#define SPEED_HALF_BYTE HIGH_HALF
#define SPEED_EEPROM_ADDR 4

#define ALT_PORT_STEPPER C
#define ALT_HALF_BYTE LOW_HALF
//#define ALT_PORT_SERVO E - niezalezne od ustawien w tym miejscu, domyslnie PORTE
#define ALT_PIN_SERVO 3
#define ALT_EEPROM_ADDR_STEPPER 6
#define ALT_EEPROM_ADDR_SERVO 8

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
#define GREEN_LED_PIN 0
#define RED_LED_PORT B
#define RED_LED_PIN 1

//****************************USTAWIENIA UART******************************//
#define BAUD_RATE 115200
#define CONNECTION_TIMEOUT 1000 //[ms]
#define GO_TO_NEUTRUM_TIME_CONSTANT 3000 //[ms]

//****************************MAKRA POMOCNICZE****************************//
#define SDDR(x) DDR ## x
#define DDR(x) SDDR(x)
#define SPORT(x) PORT ## x
#define PORT(x) SPORT(x)

#define LOW_HALF true
#define HIGH_HALF false

#define STOP_INDEX 0
#define UP_INDEX 1
#define DOWN_INDEX 2

#define FSM_OUT0 0x05
#define FSM_OUT1 0x06
#define FSM_OUT2 0x0A
#define FSM_OUT3 0x09

#define STEPS_PER_REV 200
#define SERVO_MIN_OCR 1000
#define SERVO_MAX_OCR 4900
#define SERVO_NEUTRUM ((SERVO_MIN_OCR+SERVO_MAX_OCR)/2)

#endif /* CONFIG_H_ */