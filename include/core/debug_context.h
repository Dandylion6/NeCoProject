#pragma once
#include "utility/morse_code.h"
#include <cstdint>
#include <deque>
#include <string>


struct DebugContext
{
	std::string receiverMessage = "";
	std::deque<int32_t> frames = std::deque<int32_t>(32u, 0);
	MorseCode::Pulse pulse = MorseCode::Invalid;
	bool ignoreMainMenu = false;
};