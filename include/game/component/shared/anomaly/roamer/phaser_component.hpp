#pragma once
#include "core/data/vector2.hpp"
#include <array>
#include <cstdint>


namespace Component::Anomaly
{
/**
 * Represents a Phaser type Roamer anomaly entity.
 * 
 * Holds extra variables for phaser behaviour, such as
 * `points`. Is used with the base `Component::Anomaly::Roamer`
 * component.
 * 
 * Usage example:
 * 
 * ```cpp
 * ...
 * registry.emplace<Component::Blip>(entity, ...);
 * registry.emplace<Component::Anomaly::Roamer>(entity, ...);
 * registry.emplace<Component::Anomaly::Phaser>(entity);
 * ```
 */
struct Phaser
{
	static constexpr uint8_t MAX_POINT_COUNT = 3u;


	// ------ Members ------

	std::array<Nc::Vector2f, MAX_POINT_COUNT> points { };
	float secondsUntilNextPoint = 0.0f;
	// TODO: add support for variable point counts. For now, 2 points are used.
	uint8_t pointCount = 2u;
	uint8_t currentPointIndex = 0u;


	// ------ Constructors ------

	constexpr Phaser() noexcept = default;
};

}