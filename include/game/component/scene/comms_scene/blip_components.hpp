#pragma once
#include "core/data/vector2.hpp"
#include <array>
#include <cstdint>


namespace Component::BlipState
{
/**
 * @brief Represents a blip entity where the coordinate text is jumbled.
 * 
 * Holds variables that specify how coordinates are jumbled.
 * This component only works on blip entities.
 */
struct JumbledCoordindate final
{
	// ------ Members ------

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


	// ------ Constructors ------

	constexpr JumbledCoordindate(float stability) noexcept : 
		stability(stability) 
	{ };
};


/**
 * @brief Represents a blip entity where the coordinates are glitching.
 * 
 * Holds glitch variables for each character in the coordinate text.
 */
struct CoordinateErrorData final
{
	static constexpr Nc::Vector2f GLITCH_INTERVAL_RANGE = Nc::Vector2f(0.022f, 0.42f);
	static constexpr uint8_t CHARACTER_COUNT = 4u;


	// ------ Members ------

	std::array<float, CHARACTER_COUNT> lastGlitchTimes { };
	std::array<float, CHARACTER_COUNT> nextGlitchSeconds { };
	std::array<char, CHARACTER_COUNT> glitchedCharacters { };


	// ------ Constructors ------

	constexpr CoordinateErrorData() noexcept = default;
};


/**
 * @brief Represents a blip entity which has completely failed.
 * 
 * The blip position is unstable with the `glitchOffset` variable. 
 * The coordinate text is blank.
 */
struct CompleteFailure final
{
	static constexpr Nc::Vector2f GLITCH_INTERVAL_RANGE = Nc::Vector2f(0.012f, 0.52f);
	static constexpr float OFFSET_RANGE = 8.6f;
	

	// ------ Members ------

	Nc::Vector2f glitchedOffset = Nc::Vector2f::Zero();
	float lastGlitchTime = 0.0f;
	float nextGlitchSeconds = 0.0f;


	// ------ Constructors ------

	constexpr CompleteFailure() noexcept = default;
};

}


namespace Component
{
/**
 * @brief Represents a blip on the radar display.
 * 
 * Holds data for the blip's `state`, `remainingGlitchSeconds` and if it `isActive`.
 * It can be paired with just a `Component::Transform` or with other components for
 * more behaviour.
 * 
 * Usage example:
 * ```cpp
 * registry.emplace<Component::Transform>(entity, ...);
 * registry.emplace<Component::Blip>(entity);
 * ```
 */
struct Blip final
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


	// ------ Members ------

	float remainingGlitchSeconds = 0.0f;
	State state = Stable;
	bool isActive = true;


	// ------ Constructors ------

	constexpr Blip() noexcept = default;
};

}