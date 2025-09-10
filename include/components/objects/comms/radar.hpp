#pragma once


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
	};
}


namespace Tag
{
	struct RadarPath { };

	struct RadarArtillery { };
}