#pragma once
#include "utility/vector2.hpp"
#include <array>
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

		enum State: uint8_t
		{
			Stable,
			CoordinateJumble,
			CoordinateError,
			CompleteFailure,
		};

		struct JumbledCoordindate
		{
			static constexpr Nc::Vector2f JUMBLE_INTERVAL_RANGE = Nc::Vector2f(0.03f, 0.52f);

			float lastJumbleTime = 0.0f;
			float nextJumbleSeconds = 0.0f;
			bool flippedAxis = false;
			bool duplicateFirstAxis = false;
			bool flippedSignX = false;	
			bool flippedSignY = false;
		};
		
		struct CoordinateErrorData
		{
			static constexpr Nc::Vector2f GLITCH_INTERVAL_RANGE = Nc::Vector2f(0.024f, 0.32f);
			static constexpr uint8_t CHARACTER_COUNT = 4u;

			std::array<float, CHARACTER_COUNT> lastGlitchTimes { };
			std::array<float, CHARACTER_COUNT> nextGlitchSeconds { };
			std::array<char, CHARACTER_COUNT> glitchedCharacters { };
		};

		struct CompleteFailureData
		{
			static constexpr Nc::Vector2f GLITCH_INTERVAL_RANGE = Nc::Vector2f(0.026f, 0.78f);
			static constexpr float OFFSET_RANGE = 6.0f;
			
			Nc::Vector2f glitchedOffset = Nc::Vector2f::Zero();
			float lastGlitchTime = 0.0f;
			float nextGlitchSeconds = 0.0f;
		};
		

		float remainingGlitchSeconds = 0.0f;
		State state = Stable;
		bool isActive = true;
	};
}