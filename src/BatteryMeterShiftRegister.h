/*
	BatteryMeter
	Library for checking a battery and displaying the results on LEDs.  This
	version is for lights that are on a shift register.  For lights that are
	directly on Arduino pins, there is a separate version.
	
	Copyright (c) 2019 Lance A. Endres

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as 
	published by the Free Software Foundation, either version 3 of the 
	License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
		BatteryMeterShiftRegister(ShiftRegister74HC595<numberOfShiftRegisters> *shiftRegister, unsigned int batteryMin, unsigned int batteryMax);

		// Default destructor.
		~BatteryMeterShiftRegister();

		// Private functions.  The user need not worry about these.
	private:
		// Turns on the lights associated with the level.
		void setLights(BatteryMeter::LEVEL level);

		// Members / variables.
		// The underscore denotes a variable that belongs to the class (not a local variable).
	private:
		ShiftRegister74HC595<numberOfShiftRegisters> *_shiftRegister;
};

// Definitions are stored in another file, so we have to include it here.  This allows us to separate the interface
// and definitions while using a template.  Normally, all the code has to go into the header file for a template.
#include "BatteryMeterShiftRegister.hpp"

#endif