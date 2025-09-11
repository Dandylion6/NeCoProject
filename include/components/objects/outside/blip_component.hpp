#pragma once
#include <cstdint>


namespace Component
{
	struct Blip
	{
		enum Tweens
		{
			BlipFadeIn,
			BlipFadeOut
		};

		enum class CoordTextState: uint8_t
		{
			Stable,
			CoordinateJumble,
			CompleteFailure,
		};

		bool isActive = true;
		CoordTextState coordState = Stable;
	};
}