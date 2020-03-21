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

/*
	Example lithium battery voltages and readings:
			Readings      Voltage 
			min    max    min  max 
	Level1	552.0  614.6  2.7  3.0
	Level2	614.6  677.2  3.0  3.3
	Level3	677.2  739.8  3.3  3.6
	Level4	739.8  802.4  3.6  3.9
	Level5	802.4  865.0  3.9  4.2
*/

/*
	You must have the following libraries to run this software.

	SoftTimers by Antoine Beauchamp
		- Be careful to get the right library, there are several timer libraries and even more than one "SoftTimer" library.
		- Can be installed from Arduino IDE Library Manager.
		- https://github.com/end2endzone/SoftTimers

	ButtonSuite by Lance A. Endres
		- https://github.com/lendres/ButtonSuite-Arduino
*/

/*
	If you want debugging messages printed to the serial monitor, enable the serial monitor and
	uncomment the line:
	#define BATTERYMETERDEBUG
*/

#ifndef BATTERYMETERBASE_H
#define BATTERYMETERBASE_H

#define BATTERYMETERDEBUG

#include <Arduino.h>
#include "SimpleButton.h"
#include "BatteryMeterEnums.h"

class BatteryMeter
{
	// Constructors and destructor.
	public:
		// Constructor.
		BatteryMeter(unsigned int batteryMin, unsigned int batteryMax, Battery::LEVEL level);

		// Default destructor.
		~BatteryMeter();

	// Required setup functions.  Run these functions in your "setup" routine.
	public:
		// The sensing pin is connected to the positive side of the battery so the voltage can be read.
		void setSensingPin(unsigned int sensingPin);

		// Initialization.  Run this after the other setup functions.
		virtual void begin();

	// Optional settings.
	public:
		// Ideally, you should use the constructor for this, but if you need to modify them on the fly you can use this.
		void setMinMaxReadingValues(unsigned int batteryMin, unsigned int batteryMax);

	// Loop functions.  Run these functions in your "loop" routine as required.
	public:
		// Gets the reading from the sensing pin.
		float readSensePin();

		// Converts the sensing pin reading into a battery level.
		Battery::LEVEL getBatteryLevel();

		// Returns the battery level as a percentage.
		unsigned int getBatteryPercentage();

	// Members / variables.
	// The underscore denotes a variable that belongs to the class (not a local variable).
	protected:
		// Number of levels which is the number of output segments or LEDs.
		Battery::LEVEL		_maxLevel;

	private:
		// The reading that is considered fully discharged.  This is 2.7 volts for a lithium battery.
		unsigned int			_batteryMin;

		// The reading that is considered fully charged.  This is 4.2 volts for a lithium battery.
		unsigned int			_batteryMax;

		// Analog sensing pin.  The battery level is read from this pin.
		unsigned int			_sensingPin;

		// Button that controls when the lights on activated.
		SimpleButton*			_activationButton;

		// The numerical value that each level has.  I.e., (batteryMax-batteryMin)/numberOfLevls.
		float					_levelWidth;
};

#endif