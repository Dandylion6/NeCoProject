#pragma once
#include "utility/morse_code.hpp"
#include <cstdint>
#include <deque>
#include <string>


struct DebugContext
{
	std::string receiverMessage = "";
	std::deque<int32_t> frames = std::deque<int32_t>(32u, 0);
	MorseCode::Pulse pulse = MorseCode::Invalid;
	float radarStabilityPercentage = 0.0f;
	float timeScale = 1.0f;
	bool ignoreMainMenu = false;
	bool isMaximizedWindowed = false;
	bool isRadarActiveOnStart = false;
};