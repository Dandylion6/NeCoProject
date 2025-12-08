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
 * 
 * ```cpp
 * registry.emplace<Component::Transform>(entity, ...);
 * registry.emplace<Component::Rectangle>(entity, WHITE);
 * ```
 */
struct Rectangle final
{
	// ------ Members ------

	Nc::RGBa fillColor = Nc::RGBa(WHITE);


	// ------ Constructors ------

	constexpr Rectangle() noexcept = default;
	constexpr Rectangle(Nc::RGBa fillColor) noexcept : 
		fillColor(fillColor) 
	{ };
};

}