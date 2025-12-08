#pragma once
#include <cstdint>


namespace Component
{
/**
 * @brief Represents health of an entity.
 * 
 * Stores a a 16-bit integer for health. What health represents is up 
 * how is it used and what components/systems interact with it.
 * 
 * Usage example:
 * 
 * ```cpp
 * // Each health point represents one crew member.
 * constexpr int16_t ARTILLERY_CREW_HEALTH = 3;
 * ...
 * registry.emaplce<Component::Artillery>(entity, ...);
 * registry.emplace<Component::Health>(entity, ARTILLERY_CREW_HEALTH);
 * ```
 */
struct Health final
{
	// ------ Members ------

	// @brief The current health of the entity. Health is used as a broad concept.
	int16_t health = 10;


	// ------ Constructors ------

	constexpr Health() noexcept = default;
	constexpr Health(int16_t health) noexcept : 
		health(health) 
	{ };
};

}