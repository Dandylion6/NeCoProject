#pragma once
#include "utility/morse_code.h"
#include <deque>
#include <string>


struct DebugContext
{
	std::string receiverMessage = "";
	std::deque<float> frames = std::deque<float>(32u, 0.0f);
	MorseCode::Pulse pulse = MorseCode::Invalid;
	bool ignoreMainMenu = false;
};