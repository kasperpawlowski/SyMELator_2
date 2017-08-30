/*
 * led.cpp
 *
 * Created: 05.07.2017
 *  Author: Kasper Pawlowski
 */ 

 #include <Arduino.h>
 #include "config.h"
 #include "led.h"

 void led_init()
 {
	DDR(GREEN_LED_PORT) |= 1<<GREEN_LED_PIN;
	DDR(RED_LED_PORT)	  |= 1<<RED_LED_PIN;
 }

 void led_control(const enum LedState state)
 {
	switch(state)
	{
	case BOTH_OFF:
		PORT(GREEN_LED_PORT) &= ~(1<<GREEN_LED_PIN);
		PORT(RED_LED_PORT)	 &= ~(1<<RED_LED_PIN);
		break;
	case RED_ON:
		PORT(GREEN_LED_PORT) &= ~(1<<GREEN_LED_PIN);
		PORT(RED_LED_PORT)	 |= 1<<RED_LED_PIN;
		break;
	case GREEN_ON:
		PORT(RED_LED_PORT)	 &= ~(1<<RED_LED_PIN);
		PORT(GREEN_LED_PORT) |= 1<<GREEN_LED_PIN;
		break;
	case BOTH_ON:
		PORT(RED_LED_PORT)	 |= 1<<RED_LED_PIN;
		PORT(GREEN_LED_PORT) |= 1<<GREEN_LED_PIN;
		break;
	default:
		break;
	}
 }