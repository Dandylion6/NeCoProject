#pragma once
#include "raylib.h"
#include "core/data/vector2.hpp"

namespace Component { struct Transform; }
namespace Component::UI { struct Transform; }


namespace Nc
{
	struct Bounds
	{
		Nc::Vector2f min = Nc::Vector2f::Zero();
		Nc::Vector2f max = Nc::Vector2f::Zero();

		constexpr Bounds() = default;
		constexpr Bounds(float minX, float minY, float maxX, float maxY): min(Nc::Vector2f(minX, minY)), max(Nc::Vector2f(maxX, maxY)) { };
		constexpr Bounds(Nc::Vector2f min, Nc::Vector2f max): min(min), max(max) { };
		Bounds(const Component::Transform& transform);
		Bounds(const Component::UI::Transform& transform, Nc::Vector2i screenSize);

		operator Rectangle() const;

		static bool PointInBounds(const Bounds& bounds, const Nc::Vector2f point);
		static Vector2f SizeOf(const Bounds& bounds);
		static Nc::Vector2f CenterOf(const Bounds& bounds);
	};
}