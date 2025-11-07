#pragma once
#include "core/data/vector2.hpp"


namespace Component::Action
{
/**
 * @brief Action component that provides drag state information for systems.
 * 
 * It handles drag detection with `isTarget`, records the `startPosition` 
 * and `draggedDelta` of the mouse. This takes account of the differences
 * in UI and world space.
 * 
 * Usage example:
 * 
 * ```cpp
 * registry.emplace<Component::Action::Drag>(entity);
 * registry.emplace<Component::Lever>(entity, ...);
 * ```
 */
struct Drag final
{
	// ------ Members ------

	Nc::Vector2f startPosition = Nc::Vector2f::Zero();
	Nc::Vector2f draggedDelta = Nc::Vector2f::Zero();
	bool isTarget = false;


	// ------ Constructors ------

	constexpr Drag() noexcept = default;
};

}