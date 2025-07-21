#pragma once
#include <cstdint>


enum BroadcastPriority: uint8_t
{
	Idle = 0,
	Low = 1,
	Medium = 2,
	High = 3
};


namespace Component
{
	struct Radio
	{
		float broadcastDelay = 0.0f;
		bool isSendingBroadcast = false;
		BroadcastPriority priority = Idle;

		Radio() = default;
	};
}