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
BatteryMeter::BatteryMeter() :
  _batteryMin(650),
  _batteryMax(950),
  _ledPins(NULL),
  _printDebuggingMessages(false)
{
}

BatteryMeter::BatteryMeter(bool printDebuggingMessages) :
  _ledPins(NULL),
  _printDebuggingMessages(printDebuggingMessages)
{
}

BatteryMeter::~BatteryMeter()
{
  if (_printDebuggingMessages)
  {
    Serial.println("Running destructor.");
  }
  
  if (!_ledPins)
  {
    if (_printDebuggingMessages)
    {
      Serial.println("Deleting LED pin array.");
    }
    
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
  _sensingPin         = A0;

  // Set up our input pins.
  pinMode(_sensingPin, INPUT);
  digitalWrite(_sensingPin, LOW);
}

void BatteryMeter::setActivationPin(MODE mode, unsigned int activationPin, uint8_t activationLevel)
{
  _mode             = mode;
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
  _maxLevel           = maxLevel;
  _ledPins            = new unsigned int[_maxLevel];
  _ledOnLevel         = ledOnLevel;

  // The long winded, but API appropriate way to specify write levels.
  uint8_t initialLevel =  LOW;
  if (_ledOnLevel == LOW)
  {
    initialLevel = HIGH;
  }

  // We will make a copy of the values to keep the user from accidently deleting the memory.
  for (int i = 0; i < _maxLevel; i++)
  {
    _ledPins[i] = ledPins[i];

    // Set up the light pins.
    pinMode(_ledPins[i], OUTPUT);
    digitalWrite(_ledPins[i], initialLevel);
  }
}

void BatteryMeter::begin()
{
  // The width of each level is in the units read from the sensing pin.
  _levelWidth = ((float)_batteryMax - _batteryMin) / (_maxLevel);

    // We we are dubugging, print the level.  LEVEL is zero based so we add one
  // to get the human version.
  if (_printDebuggingMessages)
  {
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
    Serial.println("");
  }
}

void BatteryMeter::runBatteryMeter()
{
  switch (_mode)
  {
    case ALWAYSON:
      meter();
      break;

//    case LATCHING:
//      break;

    case MOMENTARY:
      if (digitalRead(_activationPin) == _activationLevel)
      {
        meter();
      }
      else
      {
        setLights(LEVEL0);
      }
      break;
  }
}

bool BatteryMeter::usingDebuggingMessages()
{
  return _printDebuggingMessages;
}

float BatteryMeter::readSensePin()
{
  return analogRead(_sensingPin);
}

void BatteryMeter::meter()
{
  float sensePinReading         = analogRead(_sensingPin);
  LEVEL level                   = getBatteryLevel(sensePinReading);

  // We we are dubugging, print the level.  LEVEL is zero based so we add one
  // to get the human version.
  if (_printDebuggingMessages)
  {
    Serial.print("Sensing pin reading: ");
    Serial.println(sensePinReading);
    Serial.print("Battery level: ");
    Serial.println(level);
    Serial.println("");
  }

  setLights(level);
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

  // Something probably went wrong.
  return _maxLevel;
}

void BatteryMeter::setLights(LEVEL level)
{
  // Turn on the lights up to the current level.
  for (int i = 0; i < level; i++)
  {
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
    digitalWrite(_ledPins[i], ledOffLevel);
  }
}
