#pragma once
#include <cstdint>


namespace Component
{
	struct RadarMachine
	{
		float sability = 100.0f;
		float nextGlitchSpawnSeconds = 0.0f;
		float lastGlitchTime = 0.0f;
		uint8_t glitchCount = 0u;
		bool isActive = false;

		RadarMachine(bool isActive): isActive(isActive) { };
	};

	struct RadarErrorWarning
	{
		enum Tween 
		{
			BlinkFade = 0,
		};

		float alpha = 1.0f;
	};
}


namespace Tag
{
	struct RadarPath { };

	struct RadarArtillery { };
}