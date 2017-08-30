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

void stepper_motors_go_to_neutrum();
void clear_stepper_instrument_tab();
void release_stepper_instrument_instances();
void get_stepper_instrument_instances();
void servos_go_to_neutrum();
void clear_servo_instrument_tab();
void release_servo_instrument_instances();
void get_servo_instrument_instances();
void fsm_init();
void fsm_resume();
void fsm_stop();
void fsm_handler();
void servo_init();

#endif /* ACTUATORS_DRIVERS_H_ */