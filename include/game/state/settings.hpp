#pragma once
#include "game/utility/morse_code.hpp"


struct MorseSettings
{
	float dotTime = 0.16f;
	float dashTime = dotTime * 3.0f;
	float errorMargin = dotTime * MorseCode::ERROR_MARGIN;
	float exitTime = dashTime + errorMargin + dotTime;
};


struct Settings
{
    MorseSettings morseSettings { };

	bool operator==(const Settings& other) const
	{
		if (morseSettings.dashTime != other.morseSettings.dotTime) return false;
		return true;
	};

	static void Apply(Settings& settings);
	static void Apply(Settings& settings, Settings& toApply);
};