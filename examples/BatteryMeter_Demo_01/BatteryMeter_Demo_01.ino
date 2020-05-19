#include "BatteryMeter.h"

// DESCRIPTION
// This example shows the basics of how to run a battery meter.  This example reads the sense pin
// which is the pin the battery is connected to.  It then writes to the serial monitor the following:
// - The read value of the sense pin.
// - The battery level (the voltage between low and high is divided into several divisions, each division is a level).
// - The battery percentage full (read_value - low_voltage) / (high_voltage - low_voltage) * 100.

// CONFIGURATION
// These items should be set to the values according to how you set your Arduino up.

// The LEVEL of the battery meter is the number of levels/divisions that you have on your
// meter.  So LEVEL5 is for five outputs.
Battery::LEVEL level		= Battery::LEVEL5;

unsigned int sensePin		= A0;

// Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
// If you don't know what values to use, run the battery meter and with 2.7 volts connected and again
// with 4.2 volts, while using debug messages, and note the readings.
int batteryMin	= 550;
int batteryMax	= 850;

// EXAMPLE STARTS HERE
// Now we use all the items from the configuration section to build and run the example.

// Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
BatteryMeter batteryMeter(batteryMin, batteryMax, level);

void setup()
{
	// Configure the output serial monitor so we can write to it below.
	Serial.begin(9600);

	// Set the input sensing pin.
	batteryMeter.setSensingPin(sensePin);

	// Run the battery meter setup.
	batteryMeter.begin();
}

void loop()
{
	// These are examples of the information you can get from the battery meter.
	
	// The sense pin reading can be useful for debugging issues and to figure out what levels to set the
	// battery min and max to.
	Serial.print("Battery reading:    ");
	Serial.println(batteryMeter.readSensePin());

	// The level or percentage are the most likely functions to be used in your application.
	Serial.print("Battery level:      ");
	Serial.println(batteryMeter.getBatteryLevel());
	Serial.print("Battery percentage: ");
	Serial.println(batteryMeter.getBatteryPercentage());
  Serial.println();

	// Slow down printing so the text doesn't scroll faster than you can read it.
	delay(1000);
}
