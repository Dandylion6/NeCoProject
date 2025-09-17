#pragma once
#include "utility/vector2.hpp"
#include <array>
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
			CoordinateError,
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
		
		struct CoordinateErrorData
		{
			static constexpr Nc::Vector2i GLITCH_INTERVAL_RANGE = Nc::Vector2i(24, 320); // Interval in miliseconds.
			static constexpr uint8_t CHARACTER_COUNT = 4u;

			std::array<float, CHARACTER_COUNT> lastGlitchTimes { };
			std::array<float, CHARACTER_COUNT> nextGlitchSeconds { };
			std::array<char, CHARACTER_COUNT> glitchedCharacters { };
		};

		struct CompleteFailureData
		{
			static constexpr Nc::Vector2i GLITCH_INTERVAL_RANGE = Nc::Vector2i(260, 780); // Interval in miliseconds.
			static constexpr int OFFSET_RANGE = 1200; // Offset in 100 scale.
			
			Nc::Vector2f glitchedOffset = Nc::Vector2f::Zero();
			float lastGlitchTime = 0.0f;
			float nextGlitchSeconds = 0.0f;
		};
		

		float remainingGlitchSeconds = 0.0f;
		State state = Stable;
		bool isActive = true;
	};
}