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
						Readings     Voltage 
					 min    max   min   max 
	Level1  552.0  614.6  2.7  3.0
	Level2  614.6  677.2  3.0  3.3
	Level3  677.2  739.8  3.3  3.6
	Level4  739.8  802.4  3.6  3.9
	Level5  802.4  865.0  3.9  4.2
*/

/*
	You must have the following libraries to run this software.

	SoftTimers by Antoine Beauchamp
		- Be careful to get the right library, there are several timer libraries and even more than one "SoftTimer" library.
		- Can be installed from Arduino IDE Library Manager.
		- https://github.com/end2endzone/SoftTimers
*/

/*
	If you want debugging messages printed to the serial monitor, enable the serial monitor and
	use the following line in your file:
	
	#define BATTERYMETERDEBUG
*/

#ifndef BATTERYMETERBASE_H
#define BATTERYMETERBASE_H

#include <Arduino.h>
#include "SoftTimers.h"
#include "BatteryMeterEnums.h"

class BatteryMeterBase
{
	// Constructors and destructor.
	public:
		// Constructor.
		BatteryMeterBase(unsigned int batteryMin, unsigned int batteryMax);

		// Default destructor.
		~BatteryMeterBase();

	// Required setup functions.  Run these functions in your "setup" routine.
	public:
		// Ideally, you should use the constructor for this, but if you need to modify them on the fly you can use this.
		void setMinMaxReadingValues(unsigned int batteryMin, unsigned int batteryMax);

		// You need to set the sensing pin and activation pin.
		void setSensingPin(unsigned int sensingPin);

		// If the battery meter is activated (lights on) by a button, use this.  If you are using an
		// always on meter, you do not need to set this.
		// By setting the activation pin, you default to MOMENTARY mode.  Override by using setMode.
		void setActivationPin(unsigned int activationPin, uint8_t activationLevel);

		// Set the pins the lights are on.  The number of entries in ledPins should match the LEVEL.
		virtual void setLightPins(unsigned int ledPins[], BatteryMeter::LEVEL level, uint8_t ledOnLevel);

		// Initialization.  Run this after the other setup functions.
		void begin();

	// Optional settings.
	public:
		// Change the mode.  Initially, it is assumed ALWAYSON.  If you set the activation pin, you default to MOMENTARY.  Use
		// this to customize the behavior.
		void setMode(BatteryMeter::MODE mode);

		// The time between battery readings and updating lights.
		void setUpdateInterval(uint32_t updateInterval);

	// Loop functions.  Run these functions in your "loop" routine.
	public:
		// Entry point to the battery meter.  Checks for any state changes and updates accordingly.
		void update();

	// Debugging functions.
	public:
		// Gets the reading from the sensing pin.
		float readSensePin();

	// Protected debugging functions.  Used by the derived classes.
	protected:
		#ifdef BATTERYMETERDEBUG
		// Print the state of a pin.
		void printPinState(int pin, bool on);
		#endif

	// Private functions.  The user need not worry about these.
	private:
		// The main work of finding the level and setting the lights.
		void meter(bool forcedRun);

		// Converts the sensing pin reading into a battery level.
		BatteryMeter::LEVEL getBatteryLevel(float sensePinReading);

		// Turns on the lights associated with the level.
		virtual void setLights(BatteryMeter::LEVEL level) = 0;

	// Members / variables.
	// The underscore denotes a variable that belongs to the class (not a local variable).
	protected:
		// LED output pins.
		unsigned int*			_ledPins;

		// The level (HIGH or LOW) to use to turn the LEDs on.
		uint8_t					_ledOnLevel;

		// Number of levels which is the number of output segments or LEDs.
		BatteryMeter::LEVEL		_maxLevel;

	private:
		// The reading that is considered fully discharged.  This is 2.7 volts for a lithium battery.
		unsigned int			_batteryMin;

		// The reading that is considered fully charged.  This is 4.2 volts for a lithium battery.
		unsigned int			_batteryMax;

		// Analog sensing pin.  The battery level is read from this pin.
		unsigned int			_sensingPin;

		// Activation mode.  Specifies when and how the LEDs are turned on.
		BatteryMeter::MODE		_mode;

		// Pin for the button that turns the display (LEDs) on and off).
		unsigned int			_activationPin;
		uint8_t					_activationLevel;

		// The numerical value that each level has.  I.e., (batteryMax-batteryMin)/numberOfLevls.
		float					_levelWidth;

		// Used to remember the state.
		bool					_on;
		SoftTimer				_updateTimer;
};

#endif