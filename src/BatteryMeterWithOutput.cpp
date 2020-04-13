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

#include "BatteryMeterWithOutput.h"

// Constructor.
BatteryMeterWithOutput::BatteryMeterWithOutput(unsigned int batteryMin, unsigned int batteryMax, Battery::LEVEL maxLevel) :
	BatteryMeter(batteryMin, batteryMax, maxLevel),
	_ledPins(NULL),
	_ledOnLevel(HIGH)
{
  _updateTimer.setTimeOutTime(120000);
}

BatteryMeterWithOutput::~BatteryMeterWithOutput()
{
	if (_ledPins)
	{
		delete[] _ledPins;
	}
}

void BatteryMeterWithOutput::setActivationButton(SimpleButton* activationButton)
{
	_activationButton	= activationButton;
}

void BatteryMeterWithOutput::setActivationButton(SimpleButton& activationButton)
{
	_activationButton	= &activationButton;
}

void BatteryMeterWithOutput::setLightPins(unsigned int ledPins[], uint8_t ledOnLevel)
{
	_ledPins	= new unsigned int[_maxLevel];
	_ledOnLevel	= ledOnLevel;

	// We will make a copy of the values to keep the user from accidently deleting the memory.
	for (int i = 0; i < _maxLevel; i++)
	{
		_ledPins[i] = ledPins[i];
	}
}

void BatteryMeterWithOutput::begin()
{
	BatteryMeter::begin();

	// If the button is already on, we need to run the meter once so it starts immediately without
	// waiting for the timer to complete.  Otherwise, a delay will occur.
	BUTTONSTATUS buttonStatus = _activationButton->getStatus();
	if (buttonStatus == ISPRESSED || buttonStatus == WASPRESSED)
	{
		meter(true);
	}

	#ifdef BATTERYMETERDEBUG
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

void BatteryMeterWithOutput::setUpdateInterval(uint32_t updateInterval)
{
	_updateTimer.setTimeOutTime(updateInterval);
}

void BatteryMeterWithOutput::update()
{
	switch (_activationButton->getStatus())
	{
		case WASPRESSED:
		{
			// The button was just pressed, meter immediately.
			#ifdef BATTERYMETERDEBUG
				Serial.println("[BatteryMeter] Button Status: WASPRESSED");
			#endif
			meter(true);
			break;
		}

		case ISPRESSED:
		{
			// The button is pressed, meter when the timer is up.
			meter(false);
			break;
		}
		
		case WASSHORTPRESSED:
		case WASLONGPRESSED:
		{
			// The button was just released, meter immediately.
			#ifdef BATTERYMETERDEBUG
				Serial.println("[BatteryMeter] Button Status: WASSHORTPRESSED or WASLONGPRESSED");
			#endif
			// Get new status.
			BUTTONSTATUS newStatus = _activationButton->getStatus();
			if (newStatus == BUTTONSTATUS::NOTPRESSED)
			{
				setLights(Battery::LEVEL0);
			}
			break;
		}

		case NOTPRESSED:
		{
			// The button is not pressed, do nothing.
			break;
		}
	}
}

#ifdef BATTERYMETERDEBUG
void BatteryMeterWithOutput::printPinState(int pin, bool on)
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

void BatteryMeterWithOutput::meter(bool forcedRun)
{
	// If the timer is up we run.  If we have specified "forcedRun" it means run regardless
	// of whether the timer is up or not.
	if (_updateTimer.hasTimedOut() || forcedRun)
	{
		
		Battery::LEVEL level = getBatteryLevel();

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