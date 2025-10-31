#pragma once
#include <cstdint>


namespace Component
{
	struct Machine
	{
		/// <summary>
		/// The rate attraction percentage is reduced once the machine is inactive. Measured in percentage points per second.
		/// </summary>
		float attractionReduction = 0.0f;
		/// @brief Power usage in watts.
		uint16_t powerUsage = 0u;
		
		Machine() = default;
		Machine(float attractionReduction, uint16_t powerUsage) : 
			attractionReduction(attractionReduction), powerUsage(powerUsage) { };
	};
}