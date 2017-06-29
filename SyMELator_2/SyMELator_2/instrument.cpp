/*
 * instrument.cpp
 *
 * Created: 28.06.2017 16:33:10
 *  Author: Kasper Pawlowski
 */ 

#include "instrument.h"

 StepperInstrument::FSM_state StepperInstrument::FSM_[] = {
	 {0x05, &FSM_[0], &FSM_[1], &FSM_[3]},
	 {0x06, &FSM_[1], &FSM_[2], &FSM_[0]},
	 {0x0A, &FSM_[2], &FSM_[3], &FSM_[1]},
	 {0x09, &FSM_[3], &FSM_[0], &FSM_[2]}
 };
