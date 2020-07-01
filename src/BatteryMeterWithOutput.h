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
	Provides the base functionality for using output lights with the battery meter.
*/

#ifndef BATTERYMETERWITHOUTPUT_H
#define BATTERYMETERWITHOUTPUT_H

#include <Arduino.h>
#include "SoftTimers.h"
#include "BatteryMeter.h"

class BatteryMeterWithOutput : public BatteryMeter
{
	// Constructors and destructor.
	public:
		// Constructor.
		BatteryMeterWithOutput(unsigned int batteryMin, unsigned int batteryMax, Battery::LEVEL level);

		// Default destructor.
		~BatteryMeterWithOutput();

	// Required setup functions.  Run these functions in your "setup" routine.
	public:
		// This determines how the battery meter is activated (turns lights on).  Depending on class type passed (a class
		// derived from SimpleButton).  Both functions do the same thing, they just allow the argument to be passed in
		// different ways.
		void setActivationButton(SimpleButton* activationButton);
		void setActivationButton(SimpleButton& activationButton);

		// Set the pins the lights are on.  The number of entries in ledPins should match the LEVEL used in the constructor.
		virtual void setLightPins(unsigned int ledPins[], uint8_t ledOnLevel);

		// Initialization.  Run this after the other setup functions.
		void begin();

	// Optional settings.
	public:
		// The time between battery readings and updating lights.
		void setUpdateInterval(uint32_t updateInterval);

	// Loop functions.  Run these functions in your "loop" routine.
	public:
		// Entry point to the battery meter.  Checks for any state changes and updates accordingly.  This runs on a timer and only updates
		// on a time out.  This is prevent flickering of the lights when the battery measurement is near the border of two levels.  The
		// reading has a small variablity to it.
		void update();

		// If you need to run the meter immediately and not wait for the time out, use this version.
		void updateNow();

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

		// Turns on the lights associated with the level.
		virtual void setLights(Battery::LEVEL level) = 0;

	// Members / variables.
	// The underscore denotes a variable that belongs to the class (not a local variable).
	protected:
		// LED output pins.
		unsigned int*			_ledPins;

		// The level (HIGH or LOW) to use to turn the LEDs on.
		uint8_t					_ledOnLevel;

	private:
		// Button that controls when the lights on activated.
		SimpleButton*			_activationButton;

		// Used to prevent the meter level from bouncing around if the reading is between two levels.
		SoftTimer				_updateTimer;
};

#endif