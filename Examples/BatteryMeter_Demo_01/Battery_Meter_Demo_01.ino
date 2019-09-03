#include "BatteryMeter.h"

const int lightPins[]     = {3, 4, 5, 6, 7};
const int sensePin        = A0;
const int activationPin   = 12;

BatteryMeter meter(true);

void setup()
{
  if (meter.usingDebuggingMessages())
  {
    Serial.begin(9600);
  }
  
  // Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
  meter.setMinMaxReadingValues(552, 865);
  
  // Set the input sensing pin.
  meter.setSensingPin(sensePin);
  
  // Set the light output pins.
  meter.setLightPins(lightPins, BatteryMeter::LEVEL5, HIGH);

  // Set the activation mode and activation button.
  meter.setActivationPin(BatteryMeter::MOMENTARY, activationPin, LOW);

  // Run the battery meter setup.
  meter.begin();
}

void loop()
{
  meter.runBatteryMeter();
}
