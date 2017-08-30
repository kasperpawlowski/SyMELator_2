/*
 * input_buffer.cpp
 *
 * Created: 30.06.2017
 *  Author: Kasper Pawlowski
 */ 

 #include <Arduino.h>
 #include "input_buffer.h"

 InputBuffer* InputBuffer::instance_ = nullptr;

 InputBuffer::InputBuffer()
 {
	for(uint8_t i=0; i<BaseInstrument::NumberOfInstruments; ++i)
		observersTab_[i] = nullptr;
 }

 InputBuffer::~InputBuffer() 
 {
	if(instance_ != nullptr)
		delete instance_;
 }

 InputBuffer* InputBuffer::getInstance()
 {
	if(instance_ == nullptr)
		instance_ = new InputBuffer();

	return instance_;
 }

 void InputBuffer::attachObserversTab(BaseInstrument* bI[], const uint8_t n)
 {
	for(uint8_t i=0; i<n; ++i)
	{	
		if(bI[i] != nullptr)
			observersTab_[bI[i]->id_] = bI[i];
	}
 }
 
 bool parse_input_data()
 {
	static InputBuffer* input_buffer			= InputBuffer::getInstance();
	static InputBuffer::TransmissionState state = InputBuffer::FIRST_START_BYTE;
	static BaseInstrument::Mode transmission_mode;
	static BaseInstrument::InstrumentId instrument_id;
	static bool negative_val;
	static int data;
	
	do
	{
		int tmp_data = Serial.read();

		if(tmp_data != -1)
		{
			switch(state)
			{
			case InputBuffer::FIRST_START_BYTE:
				if(tmp_data == 0xfe)
					state = InputBuffer::SECOND_START_BYTE;
				break;	
			case InputBuffer::SECOND_START_BYTE:
				if(tmp_data == 0xfe)
					state = InputBuffer::CMD_BYTE;
				else
					state = InputBuffer::FIRST_DATA_BYTE;
				break;
			case InputBuffer::CMD_BYTE:
				if((tmp_data & 0x38) == 0)
					state = InputBuffer::FIRST_DATA_BYTE;
				else
				{
					state = InputBuffer::FIRST_START_BYTE;
					break;
				}
		
				if(tmp_data & 0x40)
					transmission_mode = BaseInstrument::CALIBRATION;
				else
					transmission_mode = BaseInstrument::TRANSMISSION;

				instrument_id = (BaseInstrument::InstrumentId)(tmp_data & 0x0f);
				negative_val = tmp_data & 0x80;
				break;
			case InputBuffer::FIRST_DATA_BYTE:
				state = InputBuffer::SECOND_DATA_BYTE;
				data = (tmp_data & 0xff) << 8;
				break;
			case InputBuffer::SECOND_DATA_BYTE:
				state = InputBuffer::FIRST_STOP_BYTE;
				data |= (tmp_data & 0xff);
				break;
			case InputBuffer::FIRST_STOP_BYTE:
				if(tmp_data == 0xfe)
					state = InputBuffer::SECOND_STOP_BYTE;
				else
					state = InputBuffer::FIRST_START_BYTE;
				break;
			case InputBuffer::SECOND_STOP_BYTE:
				if(tmp_data == 0x0d && input_buffer->observersTab_[instrument_id] != nullptr)
					input_buffer->observersTab_[instrument_id]->update(transmission_mode,
																	   data,negative_val);
				state = InputBuffer::FIRST_START_BYTE;
				break;
			}
		}
		else
			return false;
	} while(Serial.available() > 0);

	return true;
 }