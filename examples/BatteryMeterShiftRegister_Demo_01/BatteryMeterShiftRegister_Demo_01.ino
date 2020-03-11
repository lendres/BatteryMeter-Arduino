#include "BatteryMeterShiftRegister.h"
#include "MomentaryButton.h"

// DESCRIPTION
// This example shows the basics of how to run a battery meter with the output lights connected
// to the Arduino through a shift register.
//
// Activates using a momentary button.

// CONFIGURATION
// These items should be set to the values according to how you set your Arduino up.

// The number of shift registers.  This is the number of chips, not the total number of bits/ouput.  I.e.,
// one physical shift register will have 8 ouputs.  Below, we use 1 shift register which gives 8 lights to use.
const int numberOfShiftRegisters = 1;

// The LEVEL of the battery meter is the number of levels/divisions/lights that you have on your
// meter.  So LEVEL5 is for a five segment display or LEVEL3 for a 3 LED display.
Battery::LEVEL level = Battery::LEVEL8;

// The light pins are the pins on the shift register, not the pins on the Arduino.  The number
// of lights pins must be the same as the LEVEL number.  E.g. 5 lights for LEVEL5.
unsigned int lightPins[]	= {0, 1, 2, 3, 4, 5, 6, 7};

// Shift register pins.
int dataPin   = 2; 
int clockPin  = 3;
int latchPin  = 4;

// The sense pin and activation pin are pins on the Arduino.
int sensePin		= A0;
int activationPin	= 5;

// Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
// If you don't know what values to use, run the battery meter and with 2.7 volts connected and again
// with 4.2 volts, while using debug messages, and note the readings.
int batteryMin	= 650;
int batteryMax	= 975;

// EXAMPLE STARTS HERE
// Now we use all the items from the configuration section to build and run the example.

// Create the instances.  It is assumed the shift register is managed (and used) elsewhere and
// a pointer to it is passed to the battery meter.
ShiftRegister74HC595<numberOfShiftRegisters> shiftRegister(dataPin, clockPin, latchPin);
BatteryMeterShiftRegister<numberOfShiftRegisters> batteryMeter(&shiftRegister, batteryMin, batteryMax, level);

// Create the button which determines when the meter is active.
MomentaryButton activationButton(activationPin);

void setup()
{
	#ifdef BATTERYMETERDEBUG
		Serial.begin(9600);
	#endif

	// Set the input sensing pin (the pin should connect to the positve lead of the battery).
	batteryMeter.setSensingPin(sensePin);
	
	// Set the light output pins.
	batteryMeter.setLightPins(lightPins, HIGH);

	// Add the button.
	batteryMeter.setActivationButton(activationButton);

	// Run the battery meter setup.
	batteryMeter.begin();
}

void loop()
{
	batteryMeter.update();
}
