#pragma once
#include "game/utility/morse_code.hpp"
#include "raylib.h"
#include <array>
#include <cstdint>


namespace Component::Morse
{
/**
 * @brief Represents a morse transceiver entity.
 * 
 * Handles the timing and internal morse code values as `pulses`.
 */
struct Transceiver
{
	// TODO: Use input component instead of hard coding.
	static constexpr KeyboardKey INPUT_KEY = KeyboardKey::KEY_SPACE;
	static constexpr uint8_t MAX_PULSES = 8u;

	using PulseArray = std::array<MorseCode::Pulse, MAX_PULSES>;

	enum Tweens
	{
		ToneFadeIn,
		ToneFadeOut
	};


	// ------ Members ------

	PulseArray pulses { };
	float intervalSeconds = 0.0f;
	bool isInputActive = false;
	uint8_t pulseCount = 0u;


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
	constexpr MonitorRegion(Region region) noexcept : 
		region(region) 
	{ };
};

}