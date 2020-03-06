#ifndef BATTERYMETERENUMS_H
#define BATTERYMETERENUMS_H

namespace BatteryMeter
{
  // The meter levels.  This is the number of lights you want to display.
  // Example: For a 3 segment display or 3 LEDs you would use LEVEL3.
  // LEVEL0 is reserved for all lights off.
  enum LEVEL
  {
    LEVEL0,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
    LEVEL5,
    LEVEL6,
    LEVEL7,
    LEVEL8,
    LEVEL9,
    LEVEL10
  };

  // The mode for the display.
  enum MODE
  {
    // The display (LEDs) are always on.
		ALWAYSON,

		// The display state (on or off) is toggled by the activate pin.  Press once to turn
		// on, press again to turn off.
//      LATCHING,

		// The display turns on when the activate pin is pressed and turns off when the
		// activate pin is released.
		MOMENTARY
	};
}

#endif
