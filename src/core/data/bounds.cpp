#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "game/component/core/transform_component.hpp"
#include "raylib.h"


// ────── Constructors ──────

Nc::Bounds::Bounds(const Component::Transform& transform) noexcept
{
	min = transform.position - transform.offset;
	max = min + transform.size;
}


Nc::Bounds::Bounds(
	const Component::UI::Transform& transform, 
	Nc::Vector2i screenSize
) noexcept
{
	Nc::Vector2f anchorPoint = transform.anchor * screenSize;
	Nc::Vector2f position = anchorPoint + transform.offset;
	Nc::Vector2f origin = transform.origin * transform.size;

	min = position - origin;
	max = min + transform.size;
}


// ────── Utility ──────

bool Nc::Bounds::PointInBounds(
	const Bounds bounds, 
	const Nc::Vector2f point
) noexcept
{
	if (point.x < bounds.min.x || point.y < bounds.min.y) return false;
	if (point.x > bounds.max.x || point.y > bounds.max.y) return false;
	return true;
}