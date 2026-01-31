#pragma once
#include "core/data/color.hpp"
#include "raylib.h"


namespace Component
{
/**
 * @brief Represents a simple colored rectangle.
 * 
 * Renders based on the provided Transform component
 * and 'fillColor'. 
 * 
 * Usage example:
 * @code
 * registry.emplace<Component::Transform>(entity, ...);
 * registry.emplace<Component::Rectangle>(entity, WHITE);
 * @endcode
 */
struct Rectangle final
{
	// ------ Members ------

	Nc::RGBa fillColor = Nc::RGBa(WHITE);


	// ------ Constructors ------

	constexpr Rectangle() noexcept = default;
	explicit constexpr Rectangle(const Nc::RGBa fillColor) noexcept :
		fillColor(fillColor) { }

	explicit constexpr Rectangle(const Nc::Hex fillColor) noexcept :
		fillColor(Nc::RGBa(fillColor)) { }

};

}