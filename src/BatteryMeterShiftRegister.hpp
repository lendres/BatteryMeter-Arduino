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

template <uint8_t numberOfShiftRegisters>
BatteryMeterShiftRegister<numberOfShiftRegisters>::BatteryMeterShiftRegister(ShiftRegister74HC595<numberOfShiftRegisters>* shiftRegister, unsigned int batteryMin, unsigned int batteryMax) :
	BatteryMeterBase(batteryMin, batteryMax),
	_shiftRegister(shiftRegister)
{
}

template <uint8_t numberOfShiftRegisters>
BatteryMeterShiftRegister<numberOfShiftRegisters>::~BatteryMeterShiftRegister()
{
}

template <uint8_t numberOfShiftRegisters>
void BatteryMeterShiftRegister<numberOfShiftRegisters>::setLights(BatteryMeter::LEVEL level)
{
	// Turn on the lights up to the current level.  We use the "no update" version
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