#include "core/data/bounds.hpp"

#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "game/component/core/transform_component.hpp"


Nc::Bounds::Bounds(const Component::Transform& transform) noexcept
{
	min = transform.position - transform.offset;
	max = min + transform.size;
}


Nc::Bounds::Bounds(const Component::UI::Transform& transform, const Vector2i screenSize) noexcept
{
	const Vector2f anchorPoint = Vector::Modulate(transform.anchor, Vector2f(screenSize));
	const Vector2f position = anchorPoint + transform.offset;
	const Vector2f origin = Vector::Modulate(transform.origin, transform.size);

	min = position - origin;
	max = min + transform.size;
}


bool Nc::Bounds::PointInBounds(const Bounds bounds, const Vector2f point) noexcept
{
	if (point.x < bounds.min.x || point.y < bounds.min.y) return false;
	if (point.x > bounds.max.x || point.y > bounds.max.y) return false;
	return true;
}