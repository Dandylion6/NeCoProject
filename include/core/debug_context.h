#pragma once
#include <deque>
#include <string>


struct DebugContext
{
	std::string receiverMessage = "";
	std::deque<float> frames = std::deque<float>(32u, 0.0f);
	bool ignoreMainMenu = false;
};