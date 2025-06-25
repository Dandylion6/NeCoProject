#pragma once
#include <cstdint>


namespace Component
{
	struct Health
	{
		int16_t health = 10;

		Health() = default;
		Health(int16_t health): health(health) { };
	};
}