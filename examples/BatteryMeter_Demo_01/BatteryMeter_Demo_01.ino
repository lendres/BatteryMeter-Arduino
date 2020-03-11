#include "BatteryMeter.h"

// DESCRIPTION
// This example shows the basics of how to run a battery meter.

// CONFIGURATION
// These items should be set to the values according to how you set your Arduino up.

// The LEVEL of the battery meter is the number of levels/divisions that you have on your
// meter.  So LEVEL5 is for five outputs.
BatteryMeter::LEVEL level	= BatteryMeter::LEVEL5;

unsigned int sensePin		= A0;

// Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
// If you don't know what values to use, run the battery meter and with 2.7 volts connected and again
// with 4.2 volts, while using debug messages, and note the readings.
int batteryMin	= 650;
int batteryMax	= 975;

// EXAMPLE STARTS HERE
// Now we use all the items from the configuration section to build and run the example.

// Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
BatteryMeter meter(batteryMin, batteryMax, level);

void setup()
{
	// Configure the output serial monitor so we can write to it below.
	Serial.begin(9600);

	// Set the input sensing pin.
	meter.setSensingPin(sensePin);

	// Run the battery meter setup.
	meter.begin();
}

void loop()
{
	// These are examples of the information you can get from the battery meter.
	
	// The sense pin reading can be useful for debugging issues and to figure out what levels to set the
	// battery min and max to.
	Serial.print("Battery reading:    ");
	Serial.println(meter.readSensePin());

	// The level or percentage are the most likely functions to be used in your application.
	Serial.print("Battery level:      ");
	Serial.println(meter.getBatteryLevel());
	Serial.print("Battery percentage: ");
	Serial.println(meter.getBatteryPercentage());

	// Slow down printing so the text doesn't scroll faster than you can read it.
	delay(500);
}