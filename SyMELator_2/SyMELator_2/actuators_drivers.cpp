/*
 * fsm_driver.cpp
 *
 * Created: 30.06.2017
 *  Author: Kasper Pawlowski
 */ 

#include <Arduino.h>
#include "actuators_drivers.h"

StepperInstrument* stepper_motors_tab[BaseInstrument::NumberOfStepperInstruments] = {
	nullptr, nullptr, nullptr, nullptr, nullptr};
ServoInstrument* servos_tab[BaseInstrument::NumberOfServoInstruments] = {
	nullptr, nullptr, nullptr};
int tcnt4_state;

void stepper_motors_go_to_neutrum()
{
	if(*stepper_motors_tab == nullptr)
		return;

	for(uint8_t i=0; i<BaseInstrument::NumberOfStepperInstruments; ++i)
		stepper_motors_tab[i]->go_to_neutrum();
}

void clear_stepper_instrument_tab()
{
	for(uint8_t i=0; i<BaseInstrument::NumberOfStepperInstruments; ++i)
		stepper_motors_tab[i] = nullptr;
}

void get_stepper_instrument_instances()
{	
	InputBuffer* input_buffer = InputBuffer::getInstance();

	release_stepper_instrument_instances();

	cli();
	for(uint8_t i=0; i<BaseInstrument::NumberOfStepperInstruments; ++i)
		stepper_motors_tab[i] = pStepperInstrumentCreateTab[i]();

	input_buffer->attachObserversTab((BaseInstrument**)stepper_motors_tab,
									 BaseInstrument::NumberOfStepperInstruments);
	sei();
}

void release_stepper_instrument_instances()
{
	cli();
	for(uint8_t i=0; i<BaseInstrument::NumberOfStepperInstruments; ++i)
		if(stepper_motors_tab[i] != nullptr)
			delete stepper_motors_tab[i];

	clear_stepper_instrument_tab();
	sei();
}

void servos_go_to_neutrum()
{
	if(*servos_tab == nullptr)
		return;

	for(uint8_t i=0; i<BaseInstrument::NumberOfServoInstruments; ++i)
		servos_tab[i]->go_to_neutrum();
}

void clear_servo_instrument_tab()
{
	for(uint8_t i=0; i<BaseInstrument::NumberOfServoInstruments; ++i)
		servos_tab[i] = nullptr;
}

void get_servo_instrument_instances()
{
	InputBuffer* input_buffer = InputBuffer::getInstance();

	release_servo_instrument_instances();

	cli();
	for(uint8_t i=0; i<BaseInstrument::NumberOfServoInstruments; ++i)
		servos_tab[i] = pServoInstrumentCreateTab[i]();

	input_buffer->attachObserversTab((BaseInstrument**)servos_tab,
									 BaseInstrument::NumberOfServoInstruments);
	sei();
}

void release_servo_instrument_instances()
{
	cli();
	for(uint8_t i=0; i<BaseInstrument::NumberOfServoInstruments; ++i)
		if(servos_tab[i] != nullptr)
			delete servos_tab[i];

	clear_servo_instrument_tab();
	sei();
}

void fsm_init()
{
	TCCR4A = 0;						            //TIMER4 normal mode
	TCCR4B = 0;							          //TIMER4 normal mode
	TCCR4C = 0;						            //TIMER4 normal mode
	TCCR4B |= (1<<CS41) | (1<<CS40);	//ustawienie preskalera -> Timer4 / 64
	OCR4A = 2499;						          //przerwanie co 10ms (100Hz)
	TCNT4 = 0;							          //wyzerowanie rejestru zliczajacego
	TIMSK4 |= (1<<OCIE4A);		        //wlaczenie przerwania od porownania
}

void fsm_resume()
{
	TCNT4   = tcnt4_state;
	TIFR4  |= 1<<OCF4A;
	TIMSK4 |= 1<<OCIE4A;
}

void fsm_stop()
{
	tcnt4_state = TCNT4;
	TIMSK4 &= ~(1<<OCIE4A);		//wylaczenie przerwania od porownania
}

void fsm_handler()
{
	for(uint8_t i=0; i<BaseInstrument::NumberOfStepperInstruments; ++i)
	{
		int tmp_difference = stepper_motors_tab[i]->currentPos_ - stepper_motors_tab[i]->desiredPos_;

		if(tmp_difference > 0)
		{
			stepper_motors_tab[i]->currentStatePtr = stepper_motors_tab[i]->currentStatePtr->next[DOWN_INDEX];
			stepper_motors_tab[i]->currentPos_--;
		}
		else if(tmp_difference < 0)
		{
			stepper_motors_tab[i]->currentStatePtr = stepper_motors_tab[i]->currentStatePtr->next[UP_INDEX];
			stepper_motors_tab[i]->currentPos_++;
		}
		else
		{
			stepper_motors_tab[i]->currentStatePtr = stepper_motors_tab[i]->currentStatePtr->next[STOP_INDEX];

			if(stepper_motors_tab[i]->calibrationFlag)
			{
				stepper_motors_tab[i]->calibrationFlag = false;
				stepper_motors_tab[i]->neutrumPos_ = stepper_motors_tab[i]->currentStatePtr->out;
				stepper_motors_tab[i]->currentPos_ = 0;
				stepper_motors_tab[i]->desiredPos_ = 0;
			}
		}

		if(stepper_motors_tab[i]->lowerHalfOfPort_)
			*stepper_motors_tab[i]->portAddr_ = (*stepper_motors_tab[i]->portAddr_ & 0xf0) | (stepper_motors_tab[i]->currentStatePtr->out);
		else
			*stepper_motors_tab[i]->portAddr_ = (*stepper_motors_tab[i]->portAddr_ & 0x0f) | (stepper_motors_tab[i]->currentStatePtr->out << 4);
	}
}

void servo_init()
{
	TCCR3A = 0;							                            //TIMER3 normal mode
	TCCR3B = 0;					                             		//TIMER3 normal mode
	TCCR3C = 0;							                            //TIMER3 normal mode
	TCCR3A |= (1<<WGM31);		                            //ustawienie trybu Fast PWM, count up to ICR3
	TCCR3B |= (1<<WGM32) | (1<<WGM33);	                //ustawienie trybu Fast PWM, count up to ICR3
	TCCR3A |= (1<<COM3A1) | (1<<COM3B1) | (1<<COM3C1);	//set OC3A, OC3B, OC3C at BOTTOM, clear on OCR3A
	TCCR3B |= (1<<CS31);				                        //preskaler na 8

	ICR3 = 39999;						                            //czestotliwosc PWM 50Hz
	
	OCR3A = 0;
	OCR3B = 0;
	OCR3C = 0;
	TCNT3 = 0;
}

ISR(TIMER4_COMPA_vect)
{
	if(*stepper_motors_tab != nullptr)
		fsm_handler();
	TCNT4 = 0;
}