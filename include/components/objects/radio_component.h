#pragma once
#include <cstdint>


enum BroadcastPriority: int8_t
{
	Idle,
	Low,
	Medium,
	High
};


namespace Component
{
	struct Radio
	{
		BroadcastPriority broadcastPriority = Idle;
	};
}