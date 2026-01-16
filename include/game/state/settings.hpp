#pragma once


struct Settings final
{
	struct Morse final
	{
		float dotTime = 0.16f;
	};


	// ------ Members ------

	Morse morseSettings{ };


	// ------ Functions ------

	static bool Matches(const Settings& settings, const Settings& pending)
	{
		if (settings.morseSettings.dotTime != pending.morseSettings.dotTime) return false;
		return true;
	}
};
