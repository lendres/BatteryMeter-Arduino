# BatteryMeter

A library for reading a voltage from a battery.  The value can be return as a raw reading from an analog pin.  However, far more useful is the ability to read the battery voltage as a the percent charged or have it automatically converted to a battery "level."  Levels divide the battery voltage into discrete units for displaying the voltage as indicator lights (similar to number of bars on a cell phone).

The library works by reading the voltage on an analog pin and converting the value.  DC voltages between 0-5 volts can be read from a 5 volt Arduino.  For batteries with higher voltages, a voltage divider can be used to bring the voltage into the range of the Arduino.

Batteries maybe have different voltages as their maximum charged voltage.  In addition, small variations in electronics (supplied voltage, Arduino construction, voltage divider resistors, et cetera) can lead to different voltages read.  To compensate for all these potential differences, the reading levels are taken at the maximum and minimum battery voltage and those values set.  This calibrates the readings for any discrepancies.

The levels can be output from the Arduino as LEDs or some other method of displaying to the user the charged state of the battery.  For convenience, we will assume we are outputing to LEDs for here forward.  There are two output options.  The first is for turning on indicators attached to Arduino pins.  The second option outputs through a shift register.  These options are implemented as separate classes.

This suite was designed to work with my ButtonSuite class.  The different types of buttons can be used to control the output behavior of the BatteryMeter.  The ButtonSuite has several button types (always on, momentary, toggle).  By passing the BatteryMeter different classes from the ButtonSuite the BatteryMeter meter can be made to be always on, turn on when a button is held down (momentary), or toggle on and off with button pushes.  This gives the user the option to very easily change the BatteryMeter display behavior.

#### BatteryMeter
Provides functionality to measure battery level.  Reads the sense pin (pin battery is connect to) and can return the read value, the value converted to a level, or the value converted to a percentage.

#### BatteryMeterPins
Outputs battery levels to Arduino pins.  Used, for example, to turn on LEDs cooresponding to the battery charge.  One LED on would be charge the battery and five LEDs would mean the battery is full (when using 5 battery levels).

#### BatteryMeterShiftRegister
Outputs battery levels through a shift register.  Functionality is the same as BatteryMeterPins, but used when the output is on a shift register.

## Caution
Always ensure voltage level read on the analog pin is below 5 volts (for a 5 volt Arduino).  It is recommended to stay significantly below this level to avoid damaging the Arduino in case the voltage is higher than expected (for example, if the battery is over charged).  As an example, if you are reading is lithium-ion battery, the maximum voltages is commonly at 4.2 volts.  This gives you protection if the battery gets charged to 4.3 or 4.4 volts.  If you use a voltage divider to read a higher battery voltage, design it to keep the maximum voltage read below the maximum read value of the Arduino analog pin.

## About the Library
### Github Page

https://github.com/lendres/BatteryMeter-Arduino


### Prerequisites

To output through a shift register, the following library is required:

* [ButtonSuite](https://github.com/lendres/ButtonSuite-Arduino) - Arduino Button Library
* [ShiftRegister74HC595](https://timodenk.com/blog/shift-register-arduino-library/) - ShiftRegister74HC595 Arduino Library


### Installing

For information on installing Arduino libaries, see: [Arduino Libraries](http://www.arduino.cc/en/Guide/Libraries)


## Versioning

We use [SemVer](http://semver.org/) for versioning.

## Authors

* **Lance A. Endres** - [lendres](https://github.com/lendres)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
