#pragma once
#include "components/core/transform_component.h"
#include "raylib.h"
#include "utility/vector2.h"


namespace Nc
{
	struct Bounds
	{
		Nc::Vector2f min = Nc::Vector2f::Zero();
		Nc::Vector2f max = Nc::Vector2f::Zero();

		Bounds() = default;
		Bounds(Nc::Vector2f min, Nc::Vector2f max): min(min), max(max) { };
		Bounds(const Component::Transform& transform);
		Bounds(const Component::UiTransform& transform, Nc::Vector2i screenSize);

		operator Rectangle() const;

		static bool PointInBounds(const Bounds& bounds, const Nc::Vector2f point);
		static Vector2f SizeOf(const Bounds& bounds);
	};
}