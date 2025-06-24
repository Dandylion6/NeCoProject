#pragma once
#include "raylib.h"
#include "utility/morse_code.h"
#include <array>
#include <cstdint>


namespace Component
{
	struct MorseTransceiver
	{
		static constexpr KeyboardKey INPUT_KEY = KeyboardKey::KEY_SPACE;
		static constexpr uint8_t MAX_PULSES = 8u;
		using PulseArray = std::array<MorseCode::Pulse, MAX_PULSES>;

		enum Tweens
		{
			ToneFadeIn,
			ToneFadeOut
		};

		PulseArray pulses { };
		float intervalSeconds = 0.0f;
		bool isInputActive = false;
		uint8_t pulseCount = 0u;
	};
}