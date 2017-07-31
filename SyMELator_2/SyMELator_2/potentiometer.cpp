/*
 * potentiometer.cpp
 *
 * Created: 25.07.2017
 *  Author: Kasper Pawlowski
 */ 

 #include "potentiometer.h"

 const uint16_t steps_to_adc_constant = 1024*POTENTIOMETER_REV_TO_STEPPER_REV/STEPS_PER_REV;

 void potentiometer_init()
 {
	analogReference(DEFAULT);
	pinMode(POTENTIOMETER_ANALOG_PIN,INPUT_PULLUP);
 }

 void potentiometer_do_calibration()
 {
	static InputBuffer* input_buffer = InputBuffer::getInstance();
	static int last_reading = analogRead(POTENTIOMETER_ANALOG_PIN);
	bool neg_data = false;
	int tmp_reading = analogRead(POTENTIOMETER_ANALOG_PIN);
	int readings_difference = tmp_reading - last_reading;

	if(readings_difference < 0)
	{
		neg_data = true;
		readings_difference = -readings_difference;
	}

	if(readings_difference <= steps_to_adc_constant)
		return;
	else if(readings_difference > POTENTIOMETER_INGNORE_CONST)
	{
		last_reading = tmp_reading;
		return;
	}
	else 
		input_buffer->observersTab_[BaseInstrument::ALT_M]->update(BaseInstrument::CALIBRATION,readings_difference/steps_to_adc_constant,neg_data);

	last_reading = tmp_reading;
 }