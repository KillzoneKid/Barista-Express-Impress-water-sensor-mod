#include "sensor.h"
#include "player.h"

Sensor::Sensor()
	: _level(NLevels), _newLevel(NLevels), _millis(0), _reminderMillis(0), _reminderSet(false)
{
}

void Sensor::setup()
{
	PORTB |= (1 << PB0);
	PORTB |= (1 << PB1);
	DDRB &= ~(1 << PB0);
	DDRB &= ~(1 << PB1);
}

void Sensor::loop(unsigned long millis)
{
	GSensor.CheckLevel(static_cast<Level>(PINB & Unknown), millis);
	GSensor.CheckReminder(millis);
}

void Sensor::CheckLevel(Level level, unsigned long millis)
{
	// level hasn't changed
	if (level == _level)
		return;

	// level changed and we are handling it
	if (level == _newLevel)
	{
		// waiting to settle
		if (millis - _millis < _interval)
			return;

		// level changed and settled
		switch (_level = _newLevel)
		{
		case High:
			Player::PlayLevelHighFX();
			break;
		case Medium:
			return;
		case Low:
			Player::PlayLevelLowFX();
			_reminderMillis = millis;
			_reminderSet = true;
			return;
		case Unknown:
			break;
		}

		_reminderSet = false;
		return;
	}

	// level changed but not settled
	_newLevel = level;
	_millis = millis;
}

void Sensor::CheckReminder(unsigned long reminderMillis)
{
	if (!_reminderSet)
		return;

	if (reminderMillis - _reminderMillis < _reminderInterval)
		return;

	_reminderMillis = reminderMillis;
	Player::PlayLevelLowFX();
}

Sensor GSensor;