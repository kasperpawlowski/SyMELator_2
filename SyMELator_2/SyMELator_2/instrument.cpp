/*
 * instrument.cpp
 *
 * Created: 28.06.2017
 *  Author: Kasper Pawlowski
 */ 

#include <Arduino.h>
#include <EEPROM.h>
#include "instrument.h"
#include "actuators_drivers.h"

#define TEST 1

BaseInstrument::BaseInstrument(const enum InstrumentId id, const int factor, const int eAddr, volatile uint8_t* pAddr) :
	id_(id), multiplicationFactor_(factor), eepromAddr_(eAddr),	portAddr_(pAddr), 
	desiredPos_(0)
{
	if(!TEST)
	{
		EEPROM.get(eepromAddr_,neutrumPos_);
		if((id_ == ALT_KM || id_ == SLIP || id_ == TURN) 
			&& (neutrumPos_ < SERVO_OUT_MIN || neutrumPos_ > SERVO_OUT_MAX))
			neutrumPos_ = SERVO_OUT_1500US;
		else if((id_ == SPEED || id_ == ALT_M || id_ == VARIO5 || id_ == VARIO30 || id == COMPASS) 
			&& (neutrumPos_ != FSM_OUT0 || neutrumPos_ != FSM_OUT1 || neutrumPos_ != FSM_OUT2 || neutrumPos_ != FSM_OUT3))
			neutrumPos_ = FSM_OUT0;
	}
	else
	{
		neutrumPos_ = 0x05;
		if(id_ == ALT_KM || id_ == SLIP || id_ == TURN)
			neutrumPos_ = 3000;
	}
}

BaseInstrument::~BaseInstrument()
{
	if(!TEST)
		EEPROM.put(eepromAddr_,neutrumPos_);
}

 StepperInstrument::FSM_state StepperInstrument::FSM_[] = {
	 {FSM_OUT0, &FSM_[0], &FSM_[1], &FSM_[3]},
	 {FSM_OUT1, &FSM_[1], &FSM_[2], &FSM_[0]},
	 {FSM_OUT2, &FSM_[2], &FSM_[3], &FSM_[1]},
	 {FSM_OUT3, &FSM_[3], &FSM_[0], &FSM_[2]}
 };

 StepperInstrument::StepperInstrument(const enum InstrumentId id, const int factor, const int eAddr, volatile uint8_t* pAddr, const bool lowerHalf) :
	BaseInstrument(id,factor,eAddr,pAddr), lowerHalfOfPort_(lowerHalf),
	currentPos_(0), calibrationFlag(false)
 {
	volatile uint8_t* ddr = get_DDRx_from_PORTx(portAddr_);

	switch(neutrumPos_)
	{
	case FSM_OUT0:
		currentStatePtr = &FSM_[0];
		break;
	case FSM_OUT1:
		currentStatePtr = &FSM_[1];
		break;
	case FSM_OUT2:
		currentStatePtr = &FSM_[2];
		break;
	case FSM_OUT3:
		currentStatePtr = &FSM_[3];
		break;
	default:
		break;
	}
	
	if(lowerHalfOfPort_)
	{
		*ddr |= 0x0f;
		*portAddr_ = (*portAddr_ & 0xf0) | (currentStatePtr->out);
	}
	else
	{
		*ddr |= 0xf0;
		*portAddr_ = (*portAddr_ & 0x0f) | (currentStatePtr->out << 4);
	}
 }

 void StepperInstrument::update(const enum Mode mode, const uint16_t data, const bool neg_data)
 {
	fsm_stop();
	if(mode == BaseInstrument::CALIBRATION)
	{
		calibrationFlag = true;
		
		if(neg_data)
			desiredPos_ = (int)(-data);
		else
			desiredPos_ = data;
	}
	else
	{
		double tmp_val = (double)data/multiplicationFactor_;

		if(neg_data)
			desiredPos_ = toReferencePos(-tmp_val);
		else
			desiredPos_ = toReferencePos(tmp_val);
	}
	fsm_resume();
 }

 int StepperInstrument::toReferencePos(const double abs_pV) const
 {
	//TODO:
	return abs_pV;
 }

 ServoInstrument::ServoInstrument(const enum InstrumentId id, const int factor, const int eAddr, const uint8_t pin) :
	BaseInstrument(id,factor,eAddr,&PORTE)
 {
	volatile uint8_t* ddr = get_DDRx_from_PORTx(portAddr_);

	switch(pin)
	{
	case 3:
		ocrAddr = &OCR3A;
		break;
	case 4:
		ocrAddr = &OCR3B;
		break;
	case 5:
		ocrAddr = &OCR3C;
		break;
	default:
		ocrAddr = nullptr;
		break;
	}
	
	*ddr |= 1<<pin;
	desiredPos_ = neutrumPos_;
	*ocrAddr = desiredPos_;
 }

 void ServoInstrument::update(const enum Mode mode, const uint16_t data, const bool neg_data)
 {
	if(mode == BaseInstrument::CALIBRATION)
	{	
		neutrumPos_ += data*deadband_;

		if(neg_data)
			neutrumPos_ *= -1;
		desiredPos_ = neutrumPos_;
	}
	else
	{
		double tmp_val = (double)data/multiplicationFactor_;

		if(neg_data)
			desiredPos_ = toReferencePos(-tmp_val);
		else
			desiredPos_ = toReferencePos(tmp_val);
	}

	cli();
	*ocrAddr = desiredPos_;
	sei();
 }

 int ServoInstrument::toReferencePos(const double abs_pV) const
 {
	 //TODO:
	 return abs_pV;
 }

 volatile uint8_t* get_DDRx_from_PORTx(volatile uint8_t* pA)
 {
	if(pA == (uint8_t*)0x22)
		return &DDRA;
	else if(pA == (uint8_t*)0x25)
		return &DDRB;
	else if(pA == (uint8_t*)0x28)
		return &DDRC;
	else if(pA == (uint8_t*)0x2B)
		return &DDRD;
	else if(pA == (uint8_t*)0x2E)
		return &DDRE;
	else if(pA == (uint8_t*)0x31)
		return &DDRF;
	else if(pA == (uint8_t*)0x34)
		return &DDRG;
	else if(pA == (uint8_t*)0x102)
		return &DDRH;
	else if(pA == (uint8_t*)0x105)
		return &DDRJ;
	else if(pA == (uint8_t*)0x108)
		return &DDRK;
	else if(pA == (uint8_t*)0x10B)
		return &DDRL;
	else
		return nullptr;
 }

 StepperInstrument* wario5_create()
 {
	StepperInstrument* obj = new StepperInstrument(BaseInstrument::VARIO5, 10000,
	VARIO5_EEPROM_ADDR, &PORT(VARIO5_PORT_STEPPER), VARIO5_HALF_BYTE);
	return obj;
 }

 StepperInstrument* wario30_create()
 {
	StepperInstrument* obj = new StepperInstrument(BaseInstrument::VARIO30, 1000,
	VARIO30_EEPROM_ADDR, &PORT(VARIO30_PORT_STEPPER), VARIO30_HALF_BYTE);
	return obj;
 }

 StepperInstrument* speedo_create()
 {
	StepperInstrument* obj = new StepperInstrument(BaseInstrument::SPEED, 100,
	SPEEDO_EEPROM_ADDR, &PORT(SPEEDO_PORT_STEPPER), SPEEDO_HALF_BYTE);
	return obj;
 }

 StepperInstrument* alti_stepper_create()
 {
	StepperInstrument* obj = new StepperInstrument(BaseInstrument::ALT_M, 10,
	ALTI_EEPROM_ADDR_STEPPER, &PORT(ALTI_PORT_STEPPER), ALTI_HALF_BYTE);
	return obj;
 }

 StepperInstrument* compass_create()
 {
	StepperInstrument* obj = new StepperInstrument(BaseInstrument::COMPASS, 100,
	COMPASS_EEPROM_ADDR, &PORT(COMPASS_PORT_STEPPER), COMPASS_HALF_BYTE);
	return obj;
 }

 ServoInstrument* alti_servo_create()
 {
	ServoInstrument* obj = new ServoInstrument(BaseInstrument::ALT_KM, 1,
	ALTI_EEPROM_ADDR_SERVO, ALTI_PIN_SERVO);
	return obj;
 }

 ServoInstrument* turn_create()
 {
	ServoInstrument* obj = new ServoInstrument(BaseInstrument::TURN, 10000,
	TURN_EEPROM_ADDR, TURN_PIN_SERVO);
	return obj;
 }

 ServoInstrument* slip_create()
 {
	ServoInstrument* obj = new ServoInstrument(BaseInstrument::SLIP, 10000,
	SLIP_EEPROM_ADDR, SLIP_PIN_SERVO);
	return obj;
 }

 StepperInstrument* (*pStepperInstrumentCreateTab[BaseInstrument::NumberOfStepperInstruments])() = {
 wario5_create, wario30_create, speedo_create, alti_stepper_create, compass_create};

 ServoInstrument* (*pServoInstrumentCreateTab[BaseInstrument::NumberOfServoInstruments])() = {
 alti_servo_create, turn_create, slip_create};