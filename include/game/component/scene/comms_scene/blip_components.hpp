#pragma once
#include <array>
#include <cstdint>

#include "core/data/vector2.hpp"
#include "game/contexts/system_context.hpp"


namespace Component::Glitch
{
/**
 * @brief Represents a blip entity where the coordinate text is jumbled.
 * 
 * Holds variables that specify how coordinates are jumbled.
 * This component only works on blip entities.
 */
struct Distortion final
{
	// ------ Members ------

	/**
	 * @brief Range for the interval between jumbles in seconds when stability is low.
	 */
	static constexpr Nc::Vector2f JUMBLE_INTERVAL_LOW_RANGE = Nc::Vector2f(0.014f, 0.46f);
	/**
	 * @brief Range for the interval between jumbles in seconds when stability is high.
	 */
	static constexpr Nc::Vector2f JUMBLE_INTERVAL_HIGH_RANGE = Nc::Vector2f(1.8f, 6.2f);

	/**
	 * @brief Stability percentage of radar when this jumble was generated.
	 */
	float stability = 100.0f;
	float jumbleSecondsLeft = 0.0f;
	bool flippedAxis = false;
	bool duplicateFirstAxis = false;
	bool flippedSignX = false;	
	bool flippedSignY = false;


	// ------ Constructors ------

	explicit constexpr Distortion(const float stability) noexcept :
		stability(stability) { }
};


/**
 * @brief Represents a blip entity where the coordinates are glitching.
 * 
 * Holds glitch variables for each character in the coordinate text.
 */
struct SignalNoise final
{
	static constexpr Nc::Vector2f GLITCH_INTERVAL_RANGE = Nc::Vector2f(0.022f, 0.42f);
	static constexpr uint8_t CHARACTER_COUNT = 4u;


	// ------ Members ------

	std::array<float, CHARACTER_COUNT> glitchSecondsLeft { };
	std::array<char, CHARACTER_COUNT> glitchedCharacters { };


	// ------ Constructors ------

	constexpr SignalNoise() noexcept = default;
};


/**
 * @brief Represents a blip entity which has completely failed.
 * 
 * The blip position is unstable where the <c>glitchOffset</c> variable is used to visually move the blip.
 * The coordinate text is blank.
 */
struct ContactFailure final
{
	static constexpr Nc::Vector2f GLITCH_INTERVAL_RANGE = Nc::Vector2f(0.012f, 0.52f);
	static constexpr float OFFSET_RANGE = 8.6f;
	

	// ------ Members ------

	Nc::Vector2f glitchedOffset = Nc::Vector2f::Zero();
	float offsetSecondsLeft = 0.0f;


	// ------ Constructors ------

	constexpr ContactFailure() noexcept = default;
};

}


namespace Component
{
/**
 * @brief Represents a blip on the radar display.
 * 
 * Holds data for the blip's <c>state</c>, <c>remainingGlitchSeconds</c> and if it <c>isActive</c>.
 * It can be paired with just a <c>Component::Transform</c> or with other components for
 * more behavior.
 * 
 * Usage example:
 * @code
 * registry.emplace<Component::Transform>(entity, ...);
 * registry.emplace<Component::Blip>(entity);
 * @endcode
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