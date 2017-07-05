/*
 * instrument.h
 *
 * Created: 28.06.2017
 *  Author: Kasper Pawlowski
 */ 

#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "config.h"
#include "actuators_drivers.h"
#include <stdint-gcc.h>

class CoupleVal
{
private:
	volatile double physicalVal_;				//wielkosc fizyczna
	volatile int referenceVal_;					//wielkosc odniesienia (w krokach lub jednostkach rejestru)
public:
	CoupleVal(const double pV = 0.0, const int rV = 0);
	void set(const double pV, const int rV) volatile;		//konieczne jest ustawianie obu wartosci na raz
	void setZero() volatile{};
	double getPV() const;
	int getRV() const;
	int operator-(const CoupleVal& cV);						//operator odejmowania, zwracajacy ZAWSZE roznice wartosci odniesienia
	bool operator==(volatile CoupleVal& cV) volatile{};
	bool operator>(volatile CoupleVal& cV) volatile{};
	void operator++(int) volatile{};
	void operator--(int) volatile{};
};

class BaseInstrument
{
public:
	enum Mode {TRANSMISSION, CALIBRATION};
	enum InstrumentId {SPEED, ALT_M, ALT_KM, VARIO5, VARIO30, SLIP, TURN, COMPASS};
	static const int NumberOfInstruments = 8;
	static const int NumberOfStepperInstruments = 5;
	static const int NumberOfServoInstruments = NumberOfInstruments - NumberOfStepperInstruments;
private:
	const enum InstrumentId id_;				//id przyrzadu (pozwala dostosowac metody/zachowanie do konkretnego przyrzadu)
	const CoupleVal min_;						//minimalne wychylenie 
	const CoupleVal max_;						//maksymalne wychylenie
	volatile CoupleVal desiredPos_;				//pozycja docelowa w danej chwili czasu
	const int multiplicationFactor_;			//wartosc mnoznika dla przesylanej wielkosci (wynika wprost z zastosowanego w programie dzialajacym na komputerze)
	const int eepromAddr_;						//adres komorki pamieci EEPROM, w ktorej przechowywane sa dane o pozycji neutralnej
	volatile int neutrumPos_;					//pozycja neutralna (OCR dla serwomechanizmu, 0x05/06/0a/09 dla silnika krokowego) - odczytana z/zapisywana do komorki pamieci eeprom
	volatile uint8_t* portAddr_;				//adres portu, do ktorego podlaczone jest urzadzenie fizyczne (silnik/serwo)
	volatile bool calibrationFlag;				//flaga oznaczajaca tryb kalibracji - private czy public? volatile?
public:
	BaseInstrument(const enum InstrumentId id, const double min, const double max, const int factor, const int eAddr, volatile uint8_t* pAddr);
	virtual ~BaseInstrument() = 0;				//destruktor wirualny dla poprawnej kolejnosci wywolania
	virtual void update(const enum Mode mode, const int data) = 0;	//metoda uaktualniajaca pozycje docelowa
	virtual int toReferenceVal(const double abs_pV) const = 0;		//metoda mapujaca wartosc wielkosci fizycznej na wartosc odniesienia. zwraca wartosc odniesienia
	virtual double toPhysicalVal(const int delta_rV) const = 0;		//CZY KONIECZNA???
	virtual void portsInit() = 0;									//metoda inicjalizujaca peryferia danego instrumentu
	virtual void positionInit() = 0;								//metoda odczytujaca wartosc odpowiedniej komorki eeprom, inicjalizujaca wskaznik pozycje neutralna
	
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
	volatile CoupleVal currentPos_;								//pozycja chwilowa silnika krokowego
	const bool lowerHalfOfPort_;						//stala oznaczajaca czy silnik podlaczony do pinow P0-P3 (true) czy do P4-P7 (false)
	static FSM_state FSM_[4];							//tablica struktur FSM (finite state machine/automatu skonczonego) - wspoldzielona przez wszystkie instancje
	volatile FSM_state* currentStatePtr;				//wskaznik na chwilowy stan automatu skonczonego
public:
	StepperInstrument(const enum InstrumentId id, const double min, const double max, const int factor, const int eAddr, volatile uint8_t* pAddr, const bool lowerHalf);
	~StepperInstrument();								//w dekstruktorze zapis pozycji neutralnej do eeprom
	void update(const enum Mode mode, const int data);
	int toReferenceVal(const double abs_pV) const;
	double toPhysicalVal(const int delta_rV) const;
	void portsInit();
	void positionInit();

	friend void fsm_handler();
};

class ServoInstrument : public BaseInstrument
{
private:
	const int deadband_ = 100;							//wartosc strefy nieczulosci w jednostkach rejestru OCR - CZY KONIECZNE???
	volatile uint16_t* ocrAddr;							//adres rejestru OCR - przydzielic w konstruktorze na podstawie podanego pinu. port jest jednoznaczny ze wzgledu na timer i odpowiadajace mu wyjscie pwm
public:													//ocr uaktualniac w update()
	ServoInstrument(const enum InstrumentId id, const double min, const double max, const int factor, const int eAddr, const uint8_t pin);
	~ServoInstrument();									//w dekstruktorze zapis pozycji neutralnej do eeprom
	void update(const enum Mode mode, const int data);
	int toReferenceVal(const double abs_pV) const;
	double toPhysicalVal(const int delta_rV) const;
	void portsInit();
	void positionInit();
};

StepperInstrument* wario5_create();
StepperInstrument* wario30_create();
StepperInstrument* speedo_create();
StepperInstrument* alti_stepper_create();
StepperInstrument* compass_create();
ServoInstrument* alti_servo_create();
ServoInstrument* turn_create();
ServoInstrument* slip_create();

#endif // INSTRUMENT_H
