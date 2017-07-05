/*
 * main.cpp
 *
 * Created: 28.06.2017
 *	Author: Kasper Pawlowski
 */ 

#include <Arduino.h>
#include "led.h"
#include "input_buffer.h"

void setup() 
{
	led_init();
	led_control(OFF);

	Serial.begin(BAUD_RATE);

	if(!Serial)
	{
		led_control(RED);
		while(1) {;}
	}
}

void loop() 
{
	static bool initialized = false;
	static long last_time_parsed_data = 0;

	if(Serial.available() > 0)
	{
		if(!initialized)
		{
			led_control(BOTH);
			//getinstance krokowce, serwa, bufor
			//wlaczyc timery, przerwania etc
			initialized = true;
		}
		else
		{
			led_control(GREEN);
			parse_input_data();
			last_time_parsed_data = millis();
		}
	}
	else if(millis() - last_time_parsed_data > CONNECTION_TIMEOUT)
	{
		led_control(RED);
		if(initialized)
		{
			//zwolnij zasoby
			initialized = false;
		}
	}
	else
	{
		if(initialized)
			led_control(BOTH);
		else
			led_control(RED);
		delay(1);
	}
}
