/*
  BatteryMeterShiftRegister
  Library for checking a battery and displaying the results on LEDs that
  are on a shift register.
  
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

// Default contstructor.
template<uint8_t numberOfShiftRegisters>
BatteryMeterShiftRegister<numberOfShiftRegisters>::BatteryMeterShiftRegister(ShiftRegister74HC595<numberOfShiftRegisters>* shiftRegister, unsigned int batteryMin, unsigned int batteryMax) :
  BatteryMeter(batteryMin, batteryMax, false),
  _shiftRegister(shiftRegister)
{
}

template<uint8_t numberOfShiftRegisters>
BatteryMeterShiftRegister<numberOfShiftRegisters>::~BatteryMeterShiftRegister()
{
}

template<uint8_t numberOfShiftRegisters>
void BatteryMeterShiftRegister<numberOfShiftRegisters>::setLights(LEVEL level)
{
  // Turn on the lights up to the current level.  We use the "no update" verision
  // of set so we can set the values for all the lights and then activate once.
  for (int i = 0; i < level; i++)
  {
    #ifdef BATTERYMETERDEBUG
      printPinState(i, true);
    #endif
	  _shiftRegister->setNoUpdate(_ledPins[i], _ledOnLevel);
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
	  _shiftRegister->setNoUpdate(_ledPins[i], ledOffLevel);
  }

  // Now that all the bit values are set for the lights, activate them.
  _shiftRegister->updateRegisters();
}
