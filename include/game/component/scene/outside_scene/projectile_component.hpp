#pragma once
#include "core/data/vector2.hpp"


namespace Component
{
/**
 * @brief Represents a projectile entity.
 * 
 * Holds the `travelSecondsLeft` and `hitPosition`.
 * 
 * Usage example:
 * ```cpp
 * constexpr Nc::Vector2f HIT_POSITION = Nc::Vector2f(200.0f, 54.0f);
 * constexpr float TRAVEL_SECONDS = 4.0f;
 * 
 * registry.emplace<Component::Transform>(entity, ...);
 * registry.emplace<Component::Projectile>(entity, HIT_POSITION, TRAVEL_SECONDS);
 * ```
 */
struct Projectile final
{
	// ------ Members ------

	Nc::Vector2f hitPosition = Nc::Vector2f::Zero();
	float travelSecondsLeft = 0.0f;
	// TODO: Check what this means.
	bool isActive = true;


	// ------ Constructors ------

	constexpr Projectile(
		Nc::Vector2f hitPosition, float travelSecondsLeft
	) noexcept :
		hitPosition(hitPosition), travelSecondsLeft(travelSecondsLeft)
	{ };
};

}