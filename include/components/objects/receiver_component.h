#pragma once
#include "utility/morse_code.h"
#include <cstdint>
#include <string>


namespace Component
{
	struct Receiver
	{
		int8_t incomingCharacter = MorseCode::NULL_CODE;
		std::string message { };
	};
}