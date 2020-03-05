#include <BatteryMeterPins.h>

unsigned int lightPins[]     = {8, 9, 10, 11, 12};
unsigned int sensePin        = A0;
unsigned int activationPin   = 13;

// Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
BatteryMeterPins meter(552, 865, true);

void setup()
{
  if (meter.usingDebuggingMessages())
  {
    Serial.begin(9600);
  }

  // Set the input sensing pin.
  meter.setSensingPin(sensePin);
  
  // Set the light output pins.
  meter.setLightPins(lightPins, BatteryMeter::LEVEL5, HIGH);

  // Set the activation mode and activation button.
  meter.setActivationPin(activationPin, LOW);

  // Run the battery meter setup.
  meter.begin();
}

void loop()
{
  meter.update();
}
