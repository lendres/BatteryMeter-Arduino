#include "BatteryMeterPins.h"
#include "MomentaryButton.h"

// DESCRIPTION
// This example shows the basics of how to run a battery meter with the output lights connected
// directly to the Arduino pins.

// CONFIGURATION
// These items should be set to the values according to how you set your Arduino up.

// The LEVEL of the battery meter is the number of levels/divisions/lights that you have on your
// meter.  So LEVEL5 is for five outputs or LEVEL3 for a 3 LED display.
BatteryMeter::LEVEL level = BatteryMeter::LEVEL5;

unsigned int lightPins[]	= {8, 9, 10, 11, 12};
unsigned int sensePin		= A0;
unsigned int activationPin	= 5;

// Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
// If you don't know what values to use, run the battery meter and with 2.7 volts connected and again
// with 4.2 volts, while using debug messages, and note the readings.
int batteryMin	= 650;
int batteryMax	= 975;

// EXAMPLE STARTS HERE
// Now we use all the items from the configuration section to build and run the example.

// Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
BatteryMeterPins meter(batteryMin, batteryMax, level);

// Create the button which determines when the meter is active.
MomentaryButton activationButton(activationPin);

void setup()
{
  #ifdef BATTERYMETERDEBUG
		Serial.begin(9600);
  #endif

	// Set the input sensing pin.
	meter.setSensingPin(sensePin);

	// Set the light output pins.
	meter.setLightPins(lightPins, HIGH);

	// Add the button.
	meter.setActivationButton(activationButton);

	// Run the battery meter setup.
	meter.begin();
}

void loop()
{
	meter.update();
}