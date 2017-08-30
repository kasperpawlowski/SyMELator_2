/*
 * instrument.h
 *
 * Created: 28.06.2017
 *  Author: Kasper Pawlowski
 */ 

 /*uzycie wskaznikow na klasy pochodne zamiast na bazowe, brak metod get() i set()
	oraz relacja przyjazni z funkcja fsm_handler() (dla uzyskania dostepu do danych 
	prywatnych/chronionych)	wynikaja z checi uzyskania bardziej wydajnego kodu, 
	szczegolnie w instrukcji obslugi przerwania. kod staje sie przez to mniej 
	czytelny oraz niezgodny "ze	sztuka" jednak jest to konieczne w celu uzyskania 
	mozliwie szybkiego dzialania programu.*/

#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <Arduino.h>
#include "config.h"

class BaseInstrument
{
public:
	enum Mode {TRANSMISSION, CALIBRATION};
	enum InstrumentId {SPEED, ALT_M, ALT_KM, VARIO5, VARIO30, SLIP, TURN, COMPASS};
	static const uint8_t NumberOfInstruments = 8;
	static const uint8_t NumberOfStepperInstruments = 5;
	static const uint8_t NumberOfServoInstruments = NumberOfInstruments - NumberOfStepperInstruments;
	const enum InstrumentId id_;					//id przyrzadu (pozwala dostosowac metody/zachowanie do konkretnego przyrzadu)
protected:
	volatile int desiredPos_;					//pozycja docelowa w danej chwili czasu
	const int eepromAddr_;						//adres komorki pamieci EEPROM, w ktorej przechowywane sa dane o pozycji neutralnej
	volatile int neutrumPos_;					//pozycja neutralna (OCR dla serwomechanizmu, 0x05/06/0a/09 dla silnika krokowego) - odczytana z/zapisywana do komorki pamieci eeprom
	volatile uint8_t* portAddr_;				//adres portu, do ktorego podlaczone jest urzadzenie fizyczne (silnik/serwo)
public:
	BaseInstrument(const enum InstrumentId id, const int eAddr, volatile uint8_t* pAddr);
	virtual ~BaseInstrument();										//destruktor wirualny dla poprawnej kolejnosci wywolania
	virtual void update(const enum Mode mode, const uint16_t data, const bool neg_data) = 0;	//metoda uaktualniajaca pozycje docelowa
	virtual void go_to_neutrum() = 0;				//metoda wymuszajaca powrot do pozycji neutralnej
	
	friend void fsm_handler();
};

class StepperInstrument : public BaseInstrument
{
public:
	struct FSM_state
	{
		uint8_t out;				//stan wyjscia
		FSM_state* next[3];			//wskaznik na tablice kolejnych stanow (0-stop, 1-prawo, 2-lewo)
	};
private:
	volatile int currentPos_;							//pozycja chwilowa silnika krokowego
	const bool lowerHalfOfPort_;						//stala oznaczajaca czy silnik podlaczony do pinow P0-P3 (true) czy do P4-P7 (false)
	static FSM_state FSM_[4];							//tablica struktur FSM (finite state machine/automatu skonczonego) - wspoldzielona przez wszystkie instancje
	volatile FSM_state* currentStatePtr;				//wskaznik na chwilowy stan automatu skonczonego
	volatile bool calibrationFlag;						//flaga oznaczajaca tryb kalibracji
public:
	StepperInstrument(const enum InstrumentId id, const int eAddr, volatile uint8_t* pAddr, const bool lowerHalf);
	~StepperInstrument() {}								//w dekstruktorze zapis pozycji neutralnej do eeprom
	void update(const enum Mode mode, const uint16_t data, const bool neg_data);
	void go_to_neutrum();

	friend void fsm_handler();
};

class ServoInstrument : public BaseInstrument
{
private:
	const int deadband_ = 30;							//nieczulosc w jednostkach rejestru OCR
	volatile uint16_t* ocrAddr;							//adres rejestru OCR - przydzielic w konstruktorze na podstawie podanego pinu. port jest jednoznaczny ze wzgledu na timer i odpowiadajace mu wyjscie pwm
public:													//ocr uaktualniac w update()
	ServoInstrument(const enum InstrumentId id, const int eAddr, const uint8_t pin);
	~ServoInstrument() {}									//w dekstruktorze zapis pozycji neutralnej do eeprom
	void update(const enum Mode mode, const uint16_t data, const bool neg_data);
	void go_to_neutrum();
	inline void limit_to_servo_specific();				//ogranicza desiredPos_ do zakresu wlasciwego dla serwa
};

volatile uint8_t* get_DDRx_from_PORTx(volatile uint8_t* pA);

StepperInstrument* wario5_create();
StepperInstrument* wario30_create();
StepperInstrument* speed_create();
StepperInstrument* alt_stepper_create();
StepperInstrument* compass_create();
ServoInstrument* alt_servo_create();
ServoInstrument* turn_create();
ServoInstrument* slip_create();

extern StepperInstrument* (*pStepperInstrumentCreateTab[BaseInstrument::NumberOfStepperInstruments])();
extern ServoInstrument* (*pServoInstrumentCreateTab[BaseInstrument::NumberOfServoInstruments])();

#endif // INSTRUMENT_H
