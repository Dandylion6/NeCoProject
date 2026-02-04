#pragma once
#include <cstdint>

#include "raylib.h"


namespace Component::Morse
{
/**
 * @brief Represents a morse transceiver entity.
 * 
 * Handles the timing and internal morse code values as <c>pulses</c>.
 */
struct Transceiver
{
	enum Tweens
	{
		ToneFadeIn,
		ToneFadeOut
	};


	// ------ Members ------

	float intervalSeconds = 0.0f;

	/**
	 * The index to search for a character based on the morse code binary tree.
	 */
	uint16_t decodingIndex = 1;
    bool isPushed = false;
    bool inputJustChanged = false;


	// ------ Constructors ------

	constexpr Transceiver() noexcept = default;
};


struct MonitorRegion
{
	enum Region : uint8_t
	{
		Dot,
		Dash
	};


	// ------ Members ------

	Region region = Dot;


	// ------ Constructors ------

	constexpr MonitorRegion() noexcept = default;
	explicit constexpr MonitorRegion(const Region region) noexcept : region(region) { }
};

}
