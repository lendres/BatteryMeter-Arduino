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

#include "BatteryMeterBase.h"

// Constructor.
BatteryMeterBase::BatteryMeterBase(unsigned int batteryMin, unsigned int batteryMax) :
	_ledPins(NULL),
	_batteryMin(batteryMin),
	_batteryMax(batteryMax),
	_mode(BatteryMeter::ALWAYSON),
	_on(false)
{
	_updateTimer.setTimeOutTime(60000);
}

BatteryMeterBase::~BatteryMeterBase()
{
	if (_ledPins)
	{
		delete[] _ledPins;
	}
}

void BatteryMeterBase::setMinMaxReadingValues(unsigned int batteryMin, unsigned int batteryMax)
{
	_batteryMin = batteryMin;
	_batteryMax = batteryMax;
}

void BatteryMeterBase::setSensingPin(unsigned int sensingPin)
{
	_sensingPin = sensingPin;

	// Set up our input pin.
	pinMode(_sensingPin, INPUT);
	digitalWrite(_sensingPin, LOW);
}

void BatteryMeterBase::setActivationPin(unsigned int activationPin, uint8_t activationLevel)
{
	_mode				= BatteryMeter::MOMENTARY;
	_activationPin		= activationPin;
	_activationLevel	= activationLevel;

	// Set up our input pins.
	pinMode(_activationPin, INPUT);

	// The long winded, but API appropriate way to specify write levels.
	if (_activationLevel == LOW)
	{
		digitalWrite(_activationPin, HIGH);
	}
	else
	{
		digitalWrite(_activationPin, LOW);
	}
}

void BatteryMeterBase::setLightPins(unsigned int ledPins[], BatteryMeter::LEVEL maxLevel, uint8_t ledOnLevel)
{
	_maxLevel	= maxLevel;
	_ledPins	= new unsigned int[_maxLevel];
	_ledOnLevel	= ledOnLevel;

	// We will make a copy of the values to keep the user from accidently deleting the memory.
	for (int i = 0; i < _maxLevel; i++)
	{
		_ledPins[i] = ledPins[i];
	}
}

void BatteryMeterBase::begin()
{
	// The width of each level is in the units read from the sensing pin.
	_levelWidth = ((float)_batteryMax - _batteryMin) / (_maxLevel);

	// If in always on mode, we need to run the meter once so it starts immediately without
	// waiting for the timer to complete.
	if (_mode == BatteryMeter::ALWAYSON)
	{
		meter(true);
	}

	#ifdef BATTERYMETERDEBUG
		// We are dubugging, print info.
		Serial.print("[BatteryMeter] Mode: ");
		Serial.println(_mode);

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

		Serial.print("[BatteryMeter] Activation pin: ");
		Serial.println(_activationPin);

		Serial.print("[BatteryMeter] Light pins:");
		for (int i = 0; i < _maxLevel; i++)
		{
			Serial.print(" ");
			Serial.print(_ledPins[i]);
		}

		// Print end of line since we don't return after printing the pins above.
		Serial.println("");
	#endif
}

void BatteryMeterBase::update()
{
	switch (_mode)
	{
		case BatteryMeter::ALWAYSON:
			// The meter will only run if the timer is up.
			meter(false);
			break;

		case BatteryMeter::MOMENTARY:
			if (digitalRead(_activationPin) == _activationLevel)
			{
				// If it was off, we turn it on.  If it was already off, do nothing.
				// This has the effect of only updating the meter lights once per button
				// press.  This prevents the light from flickering between two levels if
				// the voltage is very close a level boundary.
				if (!_on)
				{
					_on = true;
					meter(true);
				}
			}
			else
			{
				// If on, turn off, otherwise we don't need to do anything.
				if (_on)
				{
					// We are not on (button is not pressed).
					_on = false;
					setLights(BatteryMeter::LEVEL0);
				}
			}
			break;
	}
}

void BatteryMeterBase::setMode(BatteryMeter::MODE mode)
{
	_mode = mode;
}

void BatteryMeterBase::setUpdateInterval(uint32_t updateInterval)
{
	_updateTimer.setTimeOutTime(updateInterval);
}

float BatteryMeterBase::readSensePin()
{
	return analogRead(_sensingPin);
}

#ifdef BATTERYMETERDEBUG
void BatteryMeterBase::printPinState(int pin, bool on)
{
	Serial.print("[BatteryMeter] Level: ");
	Serial.print(pin + 1);
	Serial.print("    Pin: ");
	Serial.print(_ledPins[pin]);
	Serial.print("    ");
	if (on)
	{
		Serial.println("On");
	}
	else
	{
		Serial.println("Off");
	}
}
#endif

void BatteryMeterBase::meter(bool forcedRun)
{
	// If the timer is up we run.  If we have specified "forcedRun" it means run regardless
	// of whether the timer is up or not.
	if (_updateTimer.hasTimedOut() || forcedRun)
	{
		float sensePinReading = analogRead(_sensingPin);
		BatteryMeter::LEVEL level = getBatteryLevel(sensePinReading);

		// Set the lights.
		setLights(level);

		// Restart the timer.
		_updateTimer.reset();

		// We we are dubugging, print the level.  LEVEL is zero based so we add one
		// to get the human version.
		#ifdef BATTERYMETERDEBUG
			Serial.print("[BatteryMeter] Reading: ");
			Serial.println(sensePinReading);

			Serial.print("[BatteryMeter] Battery level: ");
			Serial.println(level);
		#endif
	}
}

BatteryMeter::LEVEL BatteryMeterBase::getBatteryLevel(float sensePinReading)
{
	float currentLevelMax;

	for (int i = 0; i < _maxLevel; i++)
	{
		// The levels are ints and the width is a float, so we will recalculate from the min (instead
		// of just adding level each time) to limit rounding errors to +/-0.5.
		currentLevelMax = _batteryMin + (i + 1) * _levelWidth;

		if (sensePinReading < currentLevelMax)
		{
			// We found the current level, so return it.  No need to continue.
			return (BatteryMeter::LEVEL)(i + 1);
		}
	}

	// Over the max level.
	return _maxLevel;
}