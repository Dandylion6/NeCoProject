#pragma once
#include "utility/morse_code.h"
#include <cstdint>
#include <string>


enum TransmissionContext: int8_t
{
	OnStandby,
	AimingArtillery
};


namespace Component
{
	struct Receiver
	{
		char incomingCharacter = MorseCode::NULL_CODE;
		std::string message { };
		TransmissionContext currentContext = OnStandby;

		Receiver() = default;
	};
}