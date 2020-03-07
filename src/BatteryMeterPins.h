/*
	BatteryMeterPins
	Library for checking a battery and displaying the results on LEDs.
	
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

#ifndef BATTERYMETERPINS_H
#define BATTERYMETERPINS_H

#include <Arduino.h>
#include "BatteryMeterBase.h"

class BatteryMeterPins : public BatteryMeterBase
{
	// Constructors.
	public:
		// Constructor.
		BatteryMeterPins(unsigned int batteryMin, unsigned int batteryMax);

		// Default destructor.
		~BatteryMeterPins();

		// Setup functions.  Create your instance and run these functions in your "setup" routine.
	public:
		// Set the pins the lights are on.  The number of entries in ledPins should match the LEVEL.
		void setLightPins(unsigned int ledPins[], BatteryMeter::LEVEL level, uint8_t ledOnLevel);

		// Private functions.  The user need not worry about these.
	private:
		// Turns on the lights associated with the level.
		void setLights(BatteryMeter::LEVEL level);

		// Members / variables.
		// The underscorde denotes a variable that belongs to the class (not a local variable).
	private:
};

#endif