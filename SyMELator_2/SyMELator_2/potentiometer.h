/*
 * potentiometer.h
 *
 * Created: 25.07.2017
 *  Author: Kasper Pawlowski
 */ 

#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_

#include <Arduino.h>
#include "config.h"
#include "instrument.h"
#include "input_buffer.h"

void potentiometer_init();
void potentiometer_do_calibration();

#endif /* POTENTIOMETER_H_ */