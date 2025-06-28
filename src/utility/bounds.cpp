#include "components/core/transform_component.h"
#include "raylib.h"
#include "utility/bounds.h"
#include "utility/vector2.h"


namespace Nc
{
	Bounds::Bounds(const Component::Transform& transform)
	{
		min = transform.position - transform.offset;
		max = min + transform.size;
	}


	Bounds::Bounds(const Component::UiTransform& transform, Nc::Vector2i screenSize)
	{
		Nc::Vector2f anchorPoint = transform.anchor * screenSize;
		Nc::Vector2f position = anchorPoint + transform.offset;
		Nc::Vector2f origin = transform.origin * transform.size;

		min = position - origin;
		max = min + transform.size;
	}


	Bounds::operator Rectangle() const
	{
		Nc::Vector2f size = Bounds::SizeOf(*this);
		Rectangle rectangle {
			min.x,
			max.y,
			size.x,
			size.y
		};
		return rectangle;
	}


	bool Bounds::PointInBounds(const Bounds& bounds, const Nc::Vector2f point)
	{
		if (point.x < bounds.min.x || point.y < bounds.min.y) return false;
		if (point.x > bounds.max.x || point.y > bounds.max.y) return false;
		return true;
	}


	Vector2f Bounds::SizeOf(const Bounds& bounds)
	{
		return bounds.max - bounds.min;
	}
}