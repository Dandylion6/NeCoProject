#pragma once
#include "utility/morse_code.hpp"


struct MorseSettings
{
	float dotTime = 0.15f;
	float dashTime = dotTime * 3.0f;
	float errorMargin = dotTime * MorseCode::ERROR_MARGIN;
	float exitTime = dashTime + errorMargin + dotTime;
};


struct Settings
{
    MorseSettings morseSettings { };
};