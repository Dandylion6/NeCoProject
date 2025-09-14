#pragma once
#include "utility/vector2.hpp"
#include <cstdint>


namespace Component
{
	struct Blip
	{
		static constexpr Nc::Vector2i BASE_GLITCH_TIME_RANGE = Nc::Vector2i(10, 25); // How many seconds a blip will glitch for in seconds.

		enum Tweens
		{
			BlipFadeIn,
			BlipFadeOut
		};

		enum State: uint8_t
		{
			Stable,
			CoordinateJumble,
			CompleteFailure,
		};

		struct JumbledCoordindate
		{
			static constexpr Nc::Vector2i JUMBLE_INTERVAL_RANGE = Nc::Vector2i(30, 520); // Interval in miliseconds.

			float lastJumbleTime = 0.0f;
			float nextJumbleSeconds = 0.0f;
			bool flippedAxis = false;
			bool duplicateFirstAxis = false;
			bool flippedSignX = false;	
			bool flippedSignY = false;
		};

		float remainingGlitchSeconds = 0.0f;
		State state = Stable;
		bool isActive = true;
	};
}