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

		struct JumbledCoordindate
		{
			float lastJumbleTime = 0.0f;
			float nextJumbleSeconds = 0.0f;
			bool flippedAxis = false;
			bool duplicateFirstAxis = false;
			bool flippedSignX = false;	
			bool flippedSignY = false;
		};

		bool isActive = true;
		CoordTextState coordState = CoordTextState::Stable;
	};
}