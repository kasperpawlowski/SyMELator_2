/*
 * instrument.cpp
 *
 * Created: 28.06.2017
 *  Author: Kasper Pawlowski
 */ 

#include <Arduino.h>
#include "instrument.h"

 StepperInstrument* (*pStepperInstrumentCreateTab[BaseInstrument::NumberOfStepperInstruments])() = {
	wario5_create, wario30_create, speedo_create, alti_stepper_create, compass_create};

 ServoInstrument* (*pServoInstrumentCreateTab[BaseInstrument::NumberOfServoInstruments])() = {
	alti_servo_create, turn_create, slip_create};

 StepperInstrument::FSM_state StepperInstrument::FSM_[] = {
	 {0x05, &FSM_[0], &FSM_[1], &FSM_[3]},
	 {0x06, &FSM_[1], &FSM_[2], &FSM_[0]},
	 {0x0A, &FSM_[2], &FSM_[3], &FSM_[1]},
	 {0x09, &FSM_[3], &FSM_[0], &FSM_[2]}
 };