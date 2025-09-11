#pragma once
#include <cstdint>


namespace Component
{
	struct RadarMachine
	{
		float sability = 100.0f;
		float nextGlitchSpawnSeconds = 0.0f;
		float lastGlitchTime = 0.0f;
		bool isActive = false;

		RadarMachine(bool isActive): isActive(isActive) { };
	};
}


namespace Tag
{
	struct RadarPath { };

	struct RadarArtillery { };
}