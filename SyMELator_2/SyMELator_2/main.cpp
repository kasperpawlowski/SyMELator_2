﻿/*
 * main.cpp
 *
 * Created: 28.06.2017
 *	Author: Kasper Pawlowski
 */ 

#include <Arduino.h>
#include "led.h"
#include "actuators_drivers.h"
#include "potentiometer.h"

void setup()
{
	led_init();
	led_control(BOTH_OFF);

	Serial.begin(BAUD_RATE);

	if(!Serial)
	{
		led_control(RED_ON);
		while(true) {;}
	}

	fsm_init();
	servo_init();

	#ifdef _POTENTIOMETER
	potentiometer_init();
	#endif
}

void loop() 
{
	static bool initialized				        = false;
	static bool went_to_neutrum			      = false;
	static long last_time_parsed_data	    = 0;
	static long last_time_went_to_neutrum = 0;
	
	while(true)									//while w funkcji loop powoduje szybsze dzialanie programu nie ingerujac w 'skladnie arduino'
	{
		if(Serial.available() > 0)
		{
			if(initialized)
			{
				led_control(GREEN_ON);
				if(parse_input_data())
					last_time_parsed_data = millis();

				#ifdef _POTENTIOMETER
				potentiometer_do_calibration();
				#endif
			}
			else
			{
				led_control(BOTH_ON);
				get_stepper_instrument_instances();
				get_servo_instrument_instances();
				initialized = true;
			}
			went_to_neutrum = false;
		}
		else if(millis() - last_time_parsed_data > CONNECTION_TIMEOUT)
		{
			if(initialized && !went_to_neutrum)
			{
				stepper_motors_go_to_neutrum();
				servos_go_to_neutrum();
				last_time_went_to_neutrum = millis();
				went_to_neutrum = true;
			}
			else if(initialized && went_to_neutrum && 
					(millis() - last_time_went_to_neutrum > GO_TO_NEUTRUM_TIME_CONSTANT))
			{
				led_control(RED_ON);
				release_stepper_instrument_instances();
				release_servo_instrument_instances();
				initialized = false;
				went_to_neutrum = false;
			}
		}
		else if(!initialized)
		{
			led_control(RED_ON);
		}
	}
}
