#pragma once
#include "utility/morse_code.h"
#include <cstdint>
#include <string>


struct ReceiverCodeEvent
{
	int8_t character = MorseCode::NULL_CODE;
};

namespace Component
{
	struct Receiver
	{
		std::string receivedCode { };
	};
}