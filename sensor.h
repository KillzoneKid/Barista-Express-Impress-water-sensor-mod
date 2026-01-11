#pragma once
#include <Arduino.h>

class Sensor;
extern Sensor GSensor;

class Sensor
{
	enum Level : byte
	{
		Medium,			/* PINB: 0bxxxxxx00 == 0 (PB0: 0, PB1: 0) */
		Low,			/* PINB: 0bxxxxxx01 == 1 (PB0: 1, PB1: 0) */
		High,			/* PINB: 0bxxxxxx10 == 2 (PB0: 0, PB1: 1) */
		Unknown,		/* PINB: 0bxxxxxx11 == 3 (PB0: 1, PB1: 1) */
		NLevels
	};

	Level _level, _newLevel;
	static constexpr unsigned long _interval = 1000UL, _reminderInterval = 10000UL;
	unsigned long _millis, _reminderMillis;
	bool _reminderSet;

public:
	Sensor();

	static void setup();
	static void loop(unsigned long millis);

	void CheckLevel(Level level, unsigned long millis);
	void CheckReminder(unsigned long reminderMillis);
};