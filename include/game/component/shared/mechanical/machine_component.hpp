#pragma once
#include <cstdint>


namespace Component
{
/**
 * @brief Represents a machine entity.
 * 
 * Holds properties related to machine behavior, such as 
 * `attractionReduction` rate and `powerConsumption`.
 * 
 * Usage example:
 * 
 * ```cpp
 * constexpr float ATTRACTION_REDUCTION = 5.0f;
 * constexpr uint16_t POWER_USAGE = 150u;
 * ...
 * registry.emplace<Component::Machine>(
 *	entity, ATTRACTION_REDUCTION, POWER_USAGE
 * );
 * ```
 */
struct Machine final
{
	// ------ Members ------

	// @brief The rate attraction percentage is reduced once the machine is inactive. Measured in percentage points per second.
	float attractionReduction = 0.0f;
	//@brief Power consuption in watts while machine is powered and active.
	uint16_t powerUsage = 0u;


	// ------ Constructors ------
	
	constexpr Machine() noexcept = default;
	constexpr Machine(
		float attractionReduction,
		uint16_t powerUsage
	) noexcept : 
		attractionReduction(attractionReduction),
		powerUsage(powerUsage) 
	{ };
};

}