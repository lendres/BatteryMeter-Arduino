#include <ShiftRegister74HC595.h>
#include <BatteryMeterShiftRegister.h>

// The LEVEL of the battery meter is the number of levels/divisions/lights that you have on your
// meter.  So LEVEL5 is for a five segment display or LEVEL3 for a 3 LED display.
BatteryMeter::LEVEL level = BatteryMeter::LEVEL5;

// Shift register pins.
int dataPin   = 2; 
int clockPin  = 3;
int latchPin  = 4;

// The light pins are the pins on the shift register, not the pins on the Arduino.  The number
// of lights pins must be the same as the LEVEL number.  E.g. 5 lights for LEVEL5.
unsigned int lightPins[]     = {0, 1, 2, 3, 4};

// The sense pin and activation pin are pins on the Arduino.
unsigned int sensePin        = A0;
unsigned int activationPin   = 12;

// Set the min and max reading values that correspond to 2.7 and 4.2 volts (for a lithium battery).
// If you don't know what values to use, run the battery meter and with 2.7 volts connected and again
// with 4.2 volts, while using debug messages, and note the readings.
int batteryMin  = 650;
int batteryMax  = 975;
//int batteryMin  = 547;
//int batteryMax  = 850;

// If you want debug messages printed to the serial output.
bool debugMessages = true;


// Create the instances.  It is assumed the shift register is managed (and used) elsewhere and
// a pointer to it is passed to the battery meter.
ShiftRegister74HC595<1> shiftRegister(dataPin, clockPin, latchPin);
BatteryMeterShiftRegister<1> meter(&shiftRegister, batteryMin, batteryMax, debugMessages);


void setup()
{
  if (meter.usingDebuggingMessages())
  {
    Serial.begin(9600);
  }

  // Ensure all lights are off.
  for (int i = 0; i < 8; i++)
  {
    shiftRegister.set(i, LOW);
  }
  delay(1000);

  // Set the input sensing pin.
  meter.setSensingPin(sensePin);
  
  // Set the light output pins.
  meter.setLightPins(lightPins, level, HIGH);

  // Set the activation mode and activation button.
  //meter.setActivationPin(activationPin, LOW);
  meter.setUpdateInterval(1000);

  // Run the battery meter setup.
  meter.begin();
}

void loop()
{
  meter.update();
}
