#ifndef BATTERYMETERENUMS_H
#define BATTERYMETERENUMS_H

namespace Battery
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

} // namespace BatteryMeter

#endif