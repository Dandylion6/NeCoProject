#pragma once
#include <cstdint>


namespace Component
{
	struct RadarMachine
	{
		float sability = 100.0f;
		bool isActive = false;
	};
}


namespace Tag
{
	struct RadarPath { };

	struct RadarArtillery { };
}