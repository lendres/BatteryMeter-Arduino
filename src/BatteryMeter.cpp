/*
  BatteryMeter
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

#include "BatteryMeter.h"

// Default contstructor.
BatteryMeter::BatteryMeter(unsigned int batteryMin, unsigned int batteryMax) :
  _ledPins(NULL),
  _printDebuggingMessages(false),
  _batteryMin(batteryMin),
  _batteryMax(batteryMax),
  _mode(ALWAYSON),
  _on(false)
{
  _updateTimer.setTimeOutTime(60000);
}

BatteryMeter::BatteryMeter(unsigned int batteryMin, unsigned int batteryMax, bool printDebuggingMessages) :
  _ledPins(NULL),
  _printDebuggingMessages(printDebuggingMessages),
  _batteryMin(batteryMin),
  _batteryMax(batteryMax),
  _mode(ALWAYSON),
  _on(false)
{
  _updateTimer.setTimeOutTime(60000);
}

BatteryMeter::~BatteryMeter()
{
  if (_ledPins)
  {
	  delete[] _ledPins;
  }
}

void BatteryMeter::setMinMaxReadingValues(unsigned int batteryMin, unsigned int batteryMax)
{
  _batteryMin     = batteryMin;
  _batteryMax     = batteryMax;
}

void BatteryMeter::setSensingPin(unsigned int sensingPin)
{
  _sensingPin         = sensingPin;

  // Set up our input pin.
  pinMode(_sensingPin, INPUT);
  digitalWrite(_sensingPin, LOW);
}

void BatteryMeter::setActivationPin(unsigned int activationPin, uint8_t activationLevel)
{
  _mode = MOMENTARY;

  _activationPin    = activationPin;
  _activationLevel  = activationLevel;

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

void BatteryMeter::setLightPins(unsigned int ledPins[], LEVEL maxLevel, uint8_t ledOnLevel)
{
	_maxLevel   = maxLevel;
	_ledPins    = new unsigned int[_maxLevel];
	_ledOnLevel = ledOnLevel;

	// We will make a copy of the values to keep the user from accidently deleting the memory.
	for (int i = 0; i < _maxLevel; i++)
	{
		_ledPins[i] = ledPins[i];
	}
}

void BatteryMeter::begin()
{
  // The width of each level is in the units read from the sensing pin.
  _levelWidth = ((float)_batteryMax - _batteryMin) / (_maxLevel);

    // We we are dubugging, print info.
  if (_printDebuggingMessages)
  {
    Serial.println("");
    Serial.println("*** Battery Meter ***");

    Serial.print("Mode: ");
    Serial.println(_mode);
    
    Serial.print("Battery level reading at low: ");
    Serial.println(_batteryMin);

    Serial.print("Battery level reading at high: ");
    Serial.println(_batteryMax);

    Serial.print("Number of battery levels: ");
    Serial.println(_maxLevel);

    Serial.print("Level  width: ");
    Serial.println(_levelWidth);
    
    Serial.print("Sensing pin: ");
    Serial.println(_sensingPin);
    
    Serial.print("Activation pin: ");
    Serial.println(_activationPin);
    
    Serial.print("Light pins:");
    for (int i = 0; i < _maxLevel; i++)
    {
      Serial.print(" ");
      Serial.print(_ledPins[i]);
    }

    // Print end of line since we don't return after printing the pins above.
    Serial.println("");
  }

  // If in always on mode, we need to run the meter once so it starts immediately without
  // waiting for the timer to complete.
  if (_mode == ALWAYSON)
  {
    meter(true);
  }
}

void BatteryMeter::update()
{
  switch (_mode)
  {
    case ALWAYSON:
      // The meter will only run if the timer is up.
      meter(false);
      break;

    case MOMENTARY:
      if (digitalRead(_activationPin) == _activationLevel)
      {
        // If it was off, we turn it on.  This has the effect of only updating
        // the meter (lights) once per button press.  This prevents the light
        // from flickering if the voltage is very close the a level boundary.
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
          setLights(LEVEL0);
        }
      }
      break;
  }
}

void BatteryMeter::setMode(MODE mode)
{
  _mode             = mode;
}

void BatteryMeter::setUpdateInterval(uint32_t updateInterval)
{
  _updateTimer.setTimeOutTime(updateInterval);
}

bool BatteryMeter::usingDebuggingMessages()
{
  return _printDebuggingMessages;
}

float BatteryMeter::readSensePin()
{
  return analogRead(_sensingPin);
}

void BatteryMeter::printPinState(int pin, bool on)
{
  if (_printDebuggingMessages)
  {
    Serial.print("Level: ");
    Serial.print(pin+1);
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
}

void BatteryMeter::meter(bool forcedRun)
{
  // If the timer is up we run.  If we have specified "forcedRun" it means run regardless
  // of whether the timer is up or not.
  if (_updateTimer.hasTimedOut() || forcedRun)
  {
    float sensePinReading    = analogRead(_sensingPin);
    LEVEL level              = getBatteryLevel(sensePinReading);

    // Set the lights.
    setLights(level);

    // Restart the timer.
    _updateTimer.reset();
    
    // We we are dubugging, print the level.  LEVEL is zero based so we add one
    // to get the human version.
    if (_printDebuggingMessages)
    {
      Serial.println("");
      Serial.println("*** Battery Meter ***");
      Serial.print("Reading: ");
      Serial.println(sensePinReading);

      Serial.print("Battery level: ");
      Serial.println(level);
    }
  }
}

BatteryMeter::LEVEL BatteryMeter::getBatteryLevel(float sensePinReading)
{ 
  float currentLevelMax;

  for (int i = 0; i < _maxLevel; i++)
  {
    // The levels are ints and the width is a float, so we will recalculate from the min (instead
    // of just adding level each time) to limit rounding errors to +/-0.5.
    currentLevelMax = _batteryMin + (i+1)*_levelWidth;
    
    if (sensePinReading < currentLevelMax)
    {
      // We found the current level, so return it.  No need to continue.
      return (LEVEL)(i+1);
    }
  }

  // Over the max level.
  return _maxLevel;
}
