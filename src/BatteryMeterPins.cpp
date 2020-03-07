/*
	BatteryMeterPins
	Library for checking a battery and displaying the results on LEDs.  This
	version is for lights that are directly on Arduino pins, there is a
	separate version for lights that are on a shift register.
	
	
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

#include "BatteryMeterPins.h"

BatteryMeterPins::BatteryMeterPins(unsigned int batteryMin, unsigned int batteryMax) :
	BatteryMeterBase(batteryMin, batteryMax)
{
}

BatteryMeterPins::~BatteryMeterPins()
{
}

void BatteryMeterPins::setLightPins(unsigned int ledPins[], BatteryMeter::LEVEL maxLevel, uint8_t ledOnLevel)
{
	BatteryMeterBase::setLightPins(ledPins, maxLevel, ledOnLevel);

	// The long winded, but API appropriate way to specify write levels.
	uint8_t initialLevel = LOW;
	if (_ledOnLevel == LOW)
	{
		initialLevel = HIGH;
	}

	// Set up the light pins.
	for (int i = 0; i < _maxLevel; i++)
	{
		pinMode(_ledPins[i], OUTPUT);
		digitalWrite(_ledPins[i], initialLevel);
	}
}

void BatteryMeterPins::setLights(BatteryMeter::LEVEL level)
{
	// Turn on the lights up to the current level.
	for (int i = 0; i < level; i++)
	{
		#ifdef BATTERYMETERDEBUG
			printPinState(i, true);
		#endif
		digitalWrite(_ledPins[i], _ledOnLevel);
	}

	uint8_t ledOffLevel = LOW;
	if (_ledOnLevel == LOW)
	{
		ledOffLevel = HIGH;
	}

	// Turn off the rest of the lights.
	for (int i = level; i < _maxLevel; i++)
	{
		#ifdef BATTERYMETERDEBUG
			printPinState(i, false);
		#endif
		digitalWrite(_ledPins[i], ledOffLevel);
	}
}