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
BatteryMeterBase::BatteryMeterBase(unsigned int batteryMin, unsigned int batteryMax, BatteryMeter::LEVEL maxLevel) :
	_ledPins(NULL),
	_ledOnLevel(HIGH),
	_maxLevel(maxLevel),
	_batteryMin(batteryMin),
	_batteryMax(batteryMax)
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

void BatteryMeterBase::setSensingPin(unsigned int sensingPin)
{
	_sensingPin = sensingPin;

	// Set up our input pin.
	pinMode(_sensingPin, INPUT);
	digitalWrite(_sensingPin, LOW);
}

void BatteryMeterBase::setActivationButton(SimpleButton* activationButton)
{
	_activationButton	= activationButton;
}

void BatteryMeterBase::setActivationButton(SimpleButton &activationButton)
{
	_activationButton	= &activationButton;
}

void BatteryMeterBase::setLightPins(unsigned int ledPins[], uint8_t ledOnLevel)
{
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

	// If the button is already on, we need to run the meter once so it starts immediately without
	// waiting for the timer to complete.  Otherwise, a delay will occur.
	BUTTONSTATUS buttonStatus = _activationButton->getStatus();
	if (buttonStatus == ISPRESSED || buttonStatus == WASPRESSED)
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
	switch (_activationButton->getStatus())
	{
		case WASPRESSED:
			// The button was just pressed, meter immediately.
			meter(true);
			break;

		case ISPRESSED:
			// The button is pressed, meter when the timer is up.
			meter(false);
			break;

		default:
			// Turn the lights off.
			setLights(BatteryMeter::LEVEL0);

	}
}

float BatteryMeterBase::readSensePin()
{
	return analogRead(_sensingPin);
}

BatteryMeter::LEVEL BatteryMeterBase::getBatteryLevel()
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
			return (BatteryMeter::LEVEL)(i + 1);
		}
	}

	// Over the max level.
	return _maxLevel;
}

void BatteryMeterBase::setMinMaxReadingValues(unsigned int batteryMin, unsigned int batteryMax)
{
	_batteryMin = batteryMin;
	_batteryMax = batteryMax;
}

void BatteryMeterBase::setUpdateInterval(uint32_t updateInterval)
{
	_updateTimer.setTimeOutTime(updateInterval);
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
		
		BatteryMeter::LEVEL level = getBatteryLevel();

		// Set the lights.
		setLights(level);

		// Restart the timer.
		_updateTimer.reset();

		// We we are dubugging, print the level.  LEVEL is zero based so we add one
		// to get the human version.
		#ifdef BATTERYMETERDEBUG
			Serial.print("[BatteryMeter] Battery level: ");
			Serial.println(level);
		#endif
	}
}