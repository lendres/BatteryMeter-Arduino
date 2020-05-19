# BatteryMeter

A library for reading a voltage from a battery and displaying the results.  There are two output options.  The first is for turning on indicators attached to Arduino pins.  The second option outputs through a shift register.  These options are implemented as separate classes.

#### BatteryMeter
Provides functionality to measure battery level.  Reads the sense pin (pin battery is connect to) and can return the read value or the value converted to a level (similar to bar level on a cell phone) or a percentage.

#### BatteryMeterPins
Outputs battery levels to Arduino pins.  Used, for example, to turn on LEDs cooresponding to the battery charge.  One LED on would be charge the battery and five LEDs would mean the battery is full (when using 5 battery levels).

#### BatteryMeterShiftRegister
Outputs battery levels through a shift register.  Functionality is the same as BatteryMeterPins, but used when the output is on a shift register.


## About the Library
### Github Page

https://github.com/lendres/BatteryMeter-Arduino


### Prerequisites

To output through a shift register, the following library is required:

* [ShiftRegister74HC595](https://timodenk.com/blog/shift-register-arduino-library/) - ShiftRegister74HC595 Arduino Library


### Installing

For information on installing Arduino libaries, see: [Arduino Libraries](http://www.arduino.cc/en/Guide/Libraries)


## Versioning

We use [SemVer](http://semver.org/) for versioning.

## Authors

* **Lance A. Endres** - [lendres](https://github.com/lendres)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
