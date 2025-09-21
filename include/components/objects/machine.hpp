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
		uint16_t powerUsage = 0u; // Power usage in watts.
		bool isActive = false;
	};
}