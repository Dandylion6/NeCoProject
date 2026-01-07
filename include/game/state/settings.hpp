#pragma once


struct Settings final
{
	struct Morse final
	{
		float dotTime = 0.16f;
	};


	// ------ Members ------

    Morse morseSettings{};
};