#pragma once
#include <cstdint>


namespace Tag
{
	struct MorseMonitorPointer { };
}


namespace Component
{
	struct MorseMonitorRegion
	{
		enum Region: uint8_t
		{
			Dot,
			Dash
		};
		Region region = Dot;

		MorseMonitorRegion(Region region): region(region) { };
	};
}