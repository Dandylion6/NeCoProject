#pragma once
#include "utility/vector2.hpp"
#include <array>
#include <cstdint>


namespace Componet::BlipState
{
	struct JumbledCoordindate
		{
			// @brief Range for the interval between jumbles in seconds when stability is low.
			static constexpr Nc::Vector2f JUMBLE_INTERVAL_LOW_RANGE = Nc::Vector2f(0.014f, 0.46f);
			// @brief Range for the interval between jumbles in seconds when stability is high.
			static constexpr Nc::Vector2f JUMBLE_INTERVAL_HIGH_RANGE = Nc::Vector2f(1.8f, 6.2f);

			// @brief Stability percentage of radar when this jumble was generated.
			float stability = 100.0f;
			float lastJumbleTime = 0.0f;
			float nextJumbleSeconds = 0.0f;
			bool flippedAxis = false;
			bool duplicateFirstAxis = false;
			bool flippedSignX = false;	
			bool flippedSignY = false;

			explicit JumbledCoordindate(float stability) : stability(stability) { };
		};
		

		struct CoordinateErrorData
		{
			static constexpr Nc::Vector2f GLITCH_INTERVAL_RANGE = Nc::Vector2f(0.022f, 0.42f);
			static constexpr uint8_t CHARACTER_COUNT = 4u;

			std::array<float, CHARACTER_COUNT> lastGlitchTimes { };
			std::array<float, CHARACTER_COUNT> nextGlitchSeconds { };
			std::array<char, CHARACTER_COUNT> glitchedCharacters { };
		};


		struct CompleteFailureData
		{
			static constexpr Nc::Vector2f GLITCH_INTERVAL_RANGE = Nc::Vector2f(0.012f, 0.52f);
			static constexpr float OFFSET_RANGE = 8.6f;
			
			Nc::Vector2f glitchedOffset = Nc::Vector2f::Zero();
			float lastGlitchTime = 0.0f;
			float nextGlitchSeconds = 0.0f;
		};
}


namespace Component
{
	struct Blip
	{
		enum State: uint8_t
		{
			Stable,
			CoordinateJumble,
			CoordinateError,
			CompleteFailure,
		};

		enum Tweens
		{
			BlipFadeIn,
			BlipFadeOut
		};

		float remainingGlitchSeconds = 0.0f;
		State state = Stable;
		bool isActive = true;
	};
}