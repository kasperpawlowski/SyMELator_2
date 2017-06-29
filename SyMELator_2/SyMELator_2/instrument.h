/*
 * instrument.cpp
 *
 * Created: 28.06.2017 15:21:15
 *  Author: Kasper Pawlowski
 */ 


#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "config.h"
#include <stdint-gcc.h>

class CoupleVal
{
private:
	volatile double physicalVal_;				//wielkosc fizyczna
	volatile int referenceVal_;					//wielkosc odniesienia (w krokach lub jednostkach rejestru)
public:
	CoupleVal(const double pV = 0.0, const int rV = 0);
	void Set(const double pV, const int rV) volatile;		//konieczne jest ustawianie obu wartosci na raz
	double getPV() const;
	int getRV() const;
	int operator-(const CoupleVal& cv);						//operator odejmowania, zwracajacy ZAWSZE roznice wartosci odniesienia
};

class BaseInstrument
{
private:
	const CoupleVal min_;						//minimalne wychylenie 
	const CoupleVal max_;						//maksymalne wychylenie
	CoupleVal desiredPos_;						//pozycja docelowa w danej chwili czasu
	const int multiplicationFactor_;			//wartosc mnoznika dla przesylanej wielkosci (wynika wprost z zastosowanego w programie dzialajacym na komputerze)
	const int eepromAddr_;						//adres komorki pamieci EEPROM, w ktorej przechowywane sa dane o pozycji neutralnej
	volatile uint8_t* portAddr_;				//adres portu, do ktorego podlaczone jest urzadzenie fizyczne (silnik/serwo)
public:
	enum Mode {TRANSMISSION, CALIBRATION};
	BaseInstrument(const double min, const double max, const int factor, const int eAddr, volatile uint8_t* pAddr);
	virtual ~BaseInstrument() = 0;				//destruktor wirualny dla poprawnej kolejnosci wywolania
	volatile bool calibrationFlag;				//flaga oznaczajaca tryb kalibracji
	virtual void Update(const int mode);		//metoda uaktualniajaca pozycje docelowa - CHYBA NIEKONIECZNE PRZEDEFINIOWYWANIE???
	virtual int toReferenceVal(const double abs_pV) const = 0;		//metoda mapujaca wartosc wielkosci fizycznej na wartosc odniesienia. zwraca wartosc odniesienia
	virtual double toPhysicalVal(const int delta_rV) const = 0;		//CZY KONIECZNA???
	virtual void portsInit() = 0;									//metoda inicjalizujaca peryferia danego instrumentu
	virtual void positionInit() = 0;								//metoda odczytujaca wartosc odpowiedniej komorki eeprom, inicjalizujaca wskaznik pozycje neutralna
};

class StepperInstrument : public BaseInstrument
{
private:
	CoupleVal currentPos_;								//pozycja chwilowa silnika krokowego
	const bool lowerHalfOfPort_;						//stala oznaczajaca czy silnik podlaczony do pinow P0-P3 (true) czy do P4-P7 (false)
public:
	struct FSM_state
	{
		const uint8_t Out;			//stan wyjscia
		const FSM_state* next[3];		//wskaznik na tablice kolejnych stanow (0-stop, 1-prawo, 2-lewo)
	};

	static FSM_state FSM_[4];							//tablica struktur FSM (finite state machine/automatu skonczonego) - wspoldzielona przez wszystkie instancje
	volatile FSM_state* currentStatePtr;				//wskaznik na chwilowy stan automatu skonczonego
	StepperInstrument(const double min, const double max, const int factor, const int eAddr, volatile uint8_t* pAddr, const bool lowerHalf);
	~StepperInstrument();
	int toReferenceVal(const double abs_pV) const;
	double toPhysicalVal(const int delta_rV) const;
	void portsInit();
	void positionInit();
};

class ServoInstrument : public BaseInstrument
{
private:
	CoupleVal neutrumPos_;								//pozycja neutralna dla serwomechanizmu - odczytana z komorki pamieci eeprom
	const int deadband_ = 100;							//wartosc strefy nieczulosci w jednostkach rejestru OCR - CZY KONIECZNE???
	const uint8_t Out_;									//stan wyjscia
public:
	ServoInstrument(const double min, const double max, const int factor, const int eAddr, volatile uint8_t* pAddr, const uint8_t pin);
	~ServoInstrument();
	int toReferenceVal(const double abs_pV) const;
	double toPhysicalVal(const int delta_rV) const;
	void portsInit();
	void positionInit();
};

BaseInstrument* wario5_create();
BaseInstrument* wario30_create();
BaseInstrument* speedo_create();
BaseInstrument* alti_stepper_create();
BaseInstrument* compass_create();
BaseInstrument* alti_servo_create();
BaseInstrument* turn_create();
BaseInstrument* slip_create();

#endif // INSTRUMENT_H
