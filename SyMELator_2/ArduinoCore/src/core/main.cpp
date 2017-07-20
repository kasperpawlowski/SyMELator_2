/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() 
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR3A = 0;
	TCCR3B = 0;
	TCCR3C = 0;
	TCCR4A = 0;
	TCCR4B = 0;
	TCCR4C = 0;
	TCCR5A = 0;
	TCCR5B = 0;
	TCCR5C = 0;
	ADCSRA = 0;
}

void setupUSB() __attribute__((weak));
void setupUSB() { }

int main(void)
{
	init();
	initVariant();

#if defined(USBCON)
	USBDevice.attach();
#endif
	
	setup();
    
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
        
	return 0;
}

