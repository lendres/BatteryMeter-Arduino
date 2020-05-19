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

#include "BatteryMeter.h"

// Constructor.
BatteryMeter::BatteryMeter(unsigned int batteryMin, unsigned int batteryMax, Battery::LEVEL maxLevel) :
	_maxLevel(maxLevel),
	_batteryMin(batteryMin),
	_batteryMax(batteryMax)
{
}

BatteryMeter::~BatteryMeter()
{
}

void BatteryMeter::setSensingPin(unsigned int sensingPin)
{
	_sensingPin = sensingPin;

	// Set up our input pin.
	pinMode(_sensingPin, INPUT);
	digitalWrite(_sensingPin, LOW);
}

void BatteryMeter::begin()
{
	// The width of each level is in the units read from the sensing pin.
	_levelWidth = ((float)_batteryMax - _batteryMin) / (_maxLevel);

	#ifdef BATTERYMETERDEBUG
		// We are dubugging, print info.
		Serial.print("[BatteryMeter] Battery level reading at low: ");
		Serial.println(_batteryMin);

		Serial.print("[BatteryMeter] Battery level reading at high: ");
		Serial.println(_batteryMax);

		Serial.print("[BatteryMeter] Number of battery levels: ");
		Serial.println(_maxLevel);

		Serial.print("[BatteryMeter] Level  width: ");
		Serial.println(_levelWidth);

		Serial.print("[BatteryMeter] Sensing pin: ");
		Serial.println(_sensingPin);
	#endif
}

float BatteryMeter::readSensePin()
{
	float sensePinReading = analogRead(_sensingPin);

	#ifdef BATTERYMETERDEBUG
		Serial.print("[BatteryMeter] Reading: ");
		Serial.println(sensePinReading);
	#endif

	return sensePinReading;
}

Battery::LEVEL BatteryMeter::getBatteryLevel()
{
	float sensePinReading = analogRead(_sensingPin);

	#ifdef BATTERYMETERDEBUG
		Serial.print("[BatteryMeter] Reading: ");
		Serial.println(sensePinReading);
	#endif

	float currentLevelMax;

	for (int i = 0; i < _maxLevel; i++)
	{
		// The levels are ints and the width is a float, so we will recalculate from the min (instead
		// of just adding level each time) to limit rounding errors to +/-0.5.
		currentLevelMax = _batteryMin + (i + 1) * _levelWidth;

		if (sensePinReading < currentLevelMax)
		{
			// We found the current level, so return it.  No need to continue.
			return (Battery::LEVEL)(i + 1);
		}
	}

	// Over the max level.
	return _maxLevel;
}

unsigned int BatteryMeter::getBatteryPercentage()
{
	float sensePinReading = analogRead(_sensingPin);

	if (sensePinReading < _batteryMin)
	{
		#ifdef BATTERYMETERDEBUG
			Serial.println("[BatteryMeter] Percentage: 0");
		#endif
		return 0;
	}

	if (sensePinReading > _batteryMax)
	{
		#ifdef BATTERYMETERDEBUG
			Serial.println("[BatteryMeter] Percentage: 100");
		#endif
		return 100;
	}

	unsigned int percentage = 100.0 * (sensePinReading - _batteryMin) / (_batteryMax - _batteryMin);

	#ifdef BATTERYMETERDEBUG
		Serial.print("[BatteryMeter] Percentage: ");
		Serial.println(percentage);
	#endif

	return percentage;
}

void BatteryMeter::setMinMaxReadingValues(unsigned int batteryMin, unsigned int batteryMax)
{
	_batteryMin = batteryMin;
	_batteryMax = batteryMax;
}