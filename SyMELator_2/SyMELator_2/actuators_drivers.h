/*
 * fsm_driver.h
 *
 * Created: 30.06.2017
 *  Author: Kasper Pawlowski
 */ 


#ifndef ACTUATORS_DRIVERS_H_
#define ACTUATORS_DRIVERS_H_

#include "instrument.h"
#include "input_buffer.h"

void clear_stepper_instrument_tab();
void get_stepper_instrument_instances();
void release_stepper_instrument_instances();
void clear_servo_instrument_tab();
void get_servo_instrument_instances();
void release_servo_instrument_instances();
void fsm_init();
void fsm_resume();
void fsm_stop();
void fsm_handler();
void servo_init();
void servo_resume();
void servo_stop();

#endif /* ACTUATORS_DRIVERS_H_ */