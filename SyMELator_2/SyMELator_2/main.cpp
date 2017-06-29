/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio
class A
{
	public:
	struct FSM_state
	{
		uint8_t Out;				//stan wyjscia
		FSM_state* next[3];		//wskaznik na tablice kolejnych stanow (0-stop, 1-prawo, 2-lewo)
	};
	static FSM_state FSM_[4];
};

A::FSM_state A::FSM_[] = {
	{0x05, &FSM_[0], &FSM_[1], &FSM_[3]},
	{0x06, &FSM_[1], &FSM_[2], &FSM_[0]},
	{0x0A, &FSM_[2], &FSM_[3], &FSM_[1]},
	{0x09, &FSM_[3], &FSM_[0], &FSM_[2]}
};

A::FSM_state* currState = &A::FSM_[0];
int i = 0;

void setup() {
  // put your setup code here, to run once:
  DDRA |= 0x0f;
  PORTA &= ~0x0f;
}

void loop() {
  // put your main code here, to run repeatedly:
  PORTA = (PORTA & 0xf0) | currState->Out;
  currState = currState->next[i];
}
