/*
 * fsm_driver.h
 *
 * Created: 30.06.2017
 *  Author: Kasper Pawlowski
 */ 


#ifndef FSM_DRIVER_H_
#define FSM_DRIVER_H_

#include "instrument.h"

void get_stepper_instrument_instances();
void release_stepper_instrument_instances();
void get_servo_instrument_instances();
void release_servo_instrument_instances();
void fsm_init();
void fsm_stop();
void fsm_handler();
void servo_init();
void servo_stop();

#endif /* FSM_DRIVER_H_ */