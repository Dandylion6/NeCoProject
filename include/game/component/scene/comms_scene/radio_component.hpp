#pragma once
#include <cstdint>


/**
 * @brief An enum that indicates the importance of a broadcast.
 *
 * Can be used to determine what broadcast should or shouldn't override
 * the current one.
 */
enum class BroadcastPriority : uint8_t
{
	Idle = 0,
	Low = 1,
	Medium = 2,
	High = 3
};


namespace Component
{
/**
 * @brief Represents the radio entity.
 * 
 * Holds broadcast variables used to determine how
 * the audio is played.
 */
struct Radio final
{
	// ------ Members ------

	float broadcastDelay = 0.0f;
	BroadcastPriority priority = BroadcastPriority::Idle;
	bool isSendingBroadcast = false;


	// ------ Constructors

	constexpr Radio() noexcept = default;
};

}