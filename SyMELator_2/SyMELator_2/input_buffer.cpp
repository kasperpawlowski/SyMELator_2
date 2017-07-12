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
	delete instance_;
 }

 InputBuffer* InputBuffer::getInstance()
 {
	if(instance_ == nullptr)
		instance_ = new InputBuffer();

	return instance_;
 }

 void InputBuffer::attachObservers(BaseInstrument* bI[], const uint8_t n)
 {
	for(uint8_t i=0; i<n; ++i)
		observersTab_[bI[i]->id_] = bI[i];
 }

 void InputBuffer::handOnData(const BaseInstrument::InstrumentId id, const BaseInstrument::Mode mode, const uint16_t data, const bool neg_data)
 {
	observersTab_[id]->update(mode,data,neg_data);
 }
 
 bool parse_input_data()
 {
	static InputBuffer* input_buffer = InputBuffer::getInstance();
	static InputBuffer::TransmissionState state = InputBuffer::FIRST_START_BYTE;
	static BaseInstrument::Mode transmission_mode;
	static BaseInstrument::InstrumentId instrument_id;
	static bool negative_val;
	static bool fe_in_data;
	static int data;
	
	int tmp_data = Serial.read();

	if(tmp_data == -1)
		return false;

	switch(state)
	{
	case InputBuffer::FIRST_START_BYTE:
		if(tmp_data == 0xfe)
			state = InputBuffer::SECOND_START_BYTE;
		return true;	
	case InputBuffer::SECOND_START_BYTE:
		if(tmp_data == 0xfe)
			state = InputBuffer::CMD_BYTE;
		else
			state = InputBuffer::FIRST_DATA_BYTE;
		return true;
	case InputBuffer::CMD_BYTE:
		if(tmp_data & 0x30 == 0)
			state = InputBuffer::FIRST_DATA_BYTE;
		else
		{
			state = InputBuffer::FIRST_START_BYTE;
			return true;
		}
		
		if(tmp_data & 0x40)
			transmission_mode = BaseInstrument::CALIBRATION;
		else
			transmission_mode = BaseInstrument::TRANSMISSION;

		instrument_id = (BaseInstrument::InstrumentId)(tmp_data & 0x0f);
		negative_val = tmp_data & 0x80;
		fe_in_data = tmp_data & 0x10;
		return true;
	case InputBuffer::FIRST_DATA_BYTE:
		state = InputBuffer::SECOND_DATA_BYTE;
		data = (tmp_data & 0xff) << 8;
		return true;
	case InputBuffer::SECOND_DATA_BYTE:
		state = InputBuffer::FIRST_STOP_BYTE;
		data |= (tmp_data & 0xff);
		return true;
	case InputBuffer::FIRST_STOP_BYTE:
		if(tmp_data == 0xfe)
			state = InputBuffer::SECOND_STOP_BYTE;
		else
			state = InputBuffer::FIRST_START_BYTE;
		return true;
	case InputBuffer::SECOND_STOP_BYTE:
		if(tmp_data == 0x0d)
			input_buffer->handOnData(instrument_id,transmission_mode,data,negative_val);
		state = InputBuffer::FIRST_START_BYTE;
	default:
		return true;
	}
 }