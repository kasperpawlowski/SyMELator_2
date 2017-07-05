/*
 * input_buffer.h
 *
 * Created: 30.06.2017
 *  Author: Kasper Pawlowski
 */ 

#ifndef INPUT_BUFFER_H_
#define INPUT_BUFFER_H_

#include "instrument.h"
#include "actuators_drivers.h"

class InputBuffer
{
private:
	InputBuffer();					
	static InputBuffer* instance_;
	volatile long buffer_[BaseInstrument::NumberOfInstruments];
	BaseInstrument* observersTab_[BaseInstrument::NumberOfInstruments];
public:
	~InputBuffer() {delete instance_;}
	static InputBuffer* getInstance();
	void attachObserver(BaseInstrument* bI);
	void handOnData(const BaseInstrument::InstrumentId id, const BaseInstrument::Mode mode, const int data);
};

void parse_input_data();	//ma zmienna statyczna stanu

#endif /* INPUT_BUFFER_H_ */