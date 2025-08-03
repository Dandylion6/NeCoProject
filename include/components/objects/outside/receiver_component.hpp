#pragma once
#include "utility/morse_code.hpp"
#include <cstdint>
#include <string>


enum TransmissionContext: int8_t
{
	OnStandby,
	AimingArtillery,
	AdjustArtillery,
	FiringArtillery,
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