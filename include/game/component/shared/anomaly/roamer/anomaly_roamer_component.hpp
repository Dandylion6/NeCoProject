#pragma once
#include <cstdint>


// @brief Determines roamer behaviour.
enum class RoamerBehaviour : uint8_t
{
	Strider, // A roamer that moves stright to a point of interest at a constant speed.
	Phaser, // A roamer that teleports between 2 set points that get closer to the target.
	Phantom, // A roamer that moves fast when hidden but dissapears when close to the target completely.
	BEHAVIOUR_COUNT // Used for iteration and random selection.
};


// @brief Targets that the roamer will act based on.
enum class RoamerTarget : uint8_t
{
	Bunker, // The player bunker.
	Artillery, // The artillery base.
	TARGET_COUNT // Used for iteration and random selection.
};


namespace Component::Anomaly
{
/**
 * @brief Represents a Roamer anomaly entity.
 * 
 * Holds `behaviour` and `target` enums. Most roamers 
 * will be paired with another component that aid in
 * its logic.
 * 
 * Usage example:
 * ```cpp
 * constexpr RoamerBehaviour BEHAVIOUR = RoamerBehaviour::Strider;
 * constexpr RoamerTarget TARGET = RoamerTarget::Bunker;
 * 
 * registry.emplace<Component::Anomaly::Roamer>(entity, BEHAVIOUR, TARGET);
 * ```
 */
struct Roamer final
{
	// ------ Members ------

	RoamerBehaviour behaviour = RoamerBehaviour::Strider;
	RoamerTarget target = RoamerTarget::Bunker;


	// ------ Constructors ------

	constexpr Roamer(
		RoamerBehaviour behaviour, RoamerTarget target
	) noexcept :
		behaviour(behaviour), target(target) 
	{ };
};

}