#pragma once
#include <cstdint>


namespace Component
{
	struct RadarMachine
	{
		enum TextState: uint8_t
		{
			None,
			CoordinateJumble,
			CompleteFailure,
		};

		float sability = 100.0f;
		TextState textState = TextState::None;
		bool isActive = false;
	};
}


namespace Tag
{
	struct RadarPath { };

	struct RadarArtillery { };
}