/*
 * input_buffer.h
 *
 * Created: 30.06.2017
 *  Author: Kasper Pawlowski
 */ 

#ifndef INPUT_BUFFER_H_
#define INPUT_BUFFER_H_

#include "instrument.h"

class InputBuffer
{
private:
	InputBuffer();					
	static InputBuffer* instance_;
	BaseInstrument* observersTab_[BaseInstrument::NumberOfInstruments];
public:
	enum TransmissionState {FIRST_START_BYTE, SECOND_START_BYTE, CMD_BYTE, FIRST_DATA_BYTE, SECOND_DATA_BYTE, FIRST_STOP_BYTE, SECOND_STOP_BYTE};
	~InputBuffer();
	static InputBuffer* getInstance();
	void attachObserversTab(BaseInstrument* bI[], const uint8_t n);

	friend bool parse_input_data();
	friend void potentiometer_do_calibration();
};

bool parse_input_data();	//ma zmienna statyczna okreslajaca stan

#endif /* INPUT_BUFFER_H_ */