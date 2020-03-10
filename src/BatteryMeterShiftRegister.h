/*
	The MIT License (MIT)
	
	Copyright (c) 2019 Lance A. Endres
	
	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	Example lithium battery voltages and readings:
						Readings     Voltage 
					 min    max   min   max 
	Level1  552.0  614.6  2.7  3.0
	Level2  614.6  677.2  3.0  3.3
	Level3  677.2  739.8  3.3  3.6
	Level4  739.8  802.4  3.6  3.9
	Level5  802.4  865.0  3.9  4.2
*/

#ifndef BATTERYMETERSHIFTREGISTER_H
#define BATTERYMETERSHIFTREGISTER_H

#include <Arduino.h>
#include "BatteryMeterBase.h"
#include "ShiftRegister74HC595.h"

template <uint8_t numberOfShiftRegisters>
class BatteryMeterShiftRegister : public BatteryMeterBase
{
	// Constructors. 
	public:
		// Constructor.
		BatteryMeterShiftRegister(ShiftRegister74HC595<numberOfShiftRegisters>* shiftRegister, unsigned int batteryMin, unsigned int batteryMax, BatteryMeter::LEVEL level);

		// Default destructor.
		~BatteryMeterShiftRegister();

		// Private functions.  The user need not worry about these.
	private:
		// Turns on the lights associated with the level.
		void setLights(BatteryMeter::LEVEL level);

	// Members / variables.
	// The underscore denotes a variable that belongs to the class (not a local variable in a function).
	private:
		ShiftRegister74HC595<numberOfShiftRegisters>*	_shiftRegister;
};

// Definitions are stored in another file, so we have to include it here.  This allows us to separate the interface
// and definitions while using a template.  Normally, all the code has to go into the header file for a template.
#include "BatteryMeterShiftRegister.hpp"

#endif