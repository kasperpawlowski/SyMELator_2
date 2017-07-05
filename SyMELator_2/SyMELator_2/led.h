/*
 * led.h
 *
 * Created: 05.07.2017 
 *  Author: Kasper Pawlowski
 */ 


#ifndef LED_H_
#define LED_H_

enum LedState {OFF, RED, GREEN, BOTH};

void led_init();
void led_control(const enum LedState state);

#endif /* LED_H_ */