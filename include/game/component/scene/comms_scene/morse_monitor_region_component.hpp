#pragma once
#include <cstdint>


namespace Component
{
/**
 * @brief Represents the regions of the morse monitor gauge.
 * 
 * The region can be for the dot or dash.
 */
struct MorseMonitorRegion
{
	enum Region : uint8_t
	{
		Dot,
		Dash
	};


	// ------ Members ------

	Region region = Dot;


	// ------ Constructors ------

	constexpr MorseMonitorRegion(Region region) noexcept : 
		region(region) 
	{ };
};

}