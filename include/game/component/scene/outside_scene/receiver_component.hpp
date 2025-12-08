#pragma once
#include "game/utility/morse_code.hpp"
#include <cstdint>
#include <string>


/**
 * @brief An enum that gives context to transmissions.
 * 
 * Receivers can listen to specific codes based on its `TransmissionContext`.
 */
enum TransmissionContext : int8_t
{
	OnStandby,
	AimingArtillery,
	AdjustArtillery,
	FiringArtillery,
	RecalibrateRadar,
};


namespace Component
{
/**
 * @brief Represents a transmission receiver entity.
 * 
 * Holds `currentContext` and `message`. `incomingCharacter`
 * is modified by a tranmitter which will be processed, 
 * typically appended to the `message`. `currentContext` 
 * determines how the `message` will be interpreted.
 * 
 */
struct Receiver final
{
	// ------ Members ------

	std::string message { };
	char incomingCharacter = MorseCode::NULL_CODE;
	TransmissionContext currentContext = OnStandby;


	// ------ Constructors ------

	constexpr Receiver() noexcept = default;
};

}