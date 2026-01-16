#pragma once
#include "core/data/vector2.hpp"
#include "raylib.h"


namespace Component
{
struct Transform;
}


namespace Component::UI
{
struct Transform;
}


namespace Nc
{
/**
 * @brief Axis-aligned bounding box.
 * 
 * Represents a rectangular region defined by minimum and maximum 2D coordinates.
 * Can be constructed from world- or UI-space transforms and converted to a Raylib Rectangle.
 */
struct Bounds final
{
	// ------ Members ------

	Vector2f min = Vector2f::Zero();
	Vector2f max = Vector2f::Zero();


	// ------ Constructors ------

	constexpr Bounds() = default;
	explicit Bounds(const Component::Transform& transform) noexcept;
	Bounds(const Component::UI::Transform& transform, Vector2i screenSize) noexcept;


	constexpr Bounds(
		const float minX,
		const float minY,
		const float maxX,
		const float maxY
	) noexcept
		: min(Vector2f(minX, minY)),
		  max(Vector2f(maxX, maxY)) { };

	constexpr Bounds(const Vector2f min, const Vector2f max) noexcept : min(min), max(max) { }


	// ------ Conversion ------

	explicit constexpr operator Rectangle() const noexcept
	{
		const Vector2f size = SizeOf(*this);
		return { min.x, min.y, size.x, size.y };
	}


	// ------ Utility ------

	/**
	 * @brief Get the size (width and height) of the bounds.
	 * 
	 * @param bounds is the bounds to measure.
	 * @return Vector2f representing (width, height).
	 */
	constexpr static Vector2f SizeOf(const Bounds bounds) noexcept { return bounds.max - bounds.min; }

	/**
	 * @brief Get the geometric center of the bounds.
	 * 
	 * @param bounds is the bounds to measure.
	 * @return Vector2f representing the midpoint between min and max.
	 */
	constexpr static Vector2f CenterOf(const Bounds bounds) noexcept { return (bounds.min + bounds.max) * 0.5f; }

	/**
	 * @brief Test if a point lies within the bounds.
	 * 
	 * @param bounds is the bounds to test against.
	 * @param point is the vector2f point to test.
	 * @return True if point is inside or on the boundary, false otherwise.
	 */
	static bool PointInBounds(Bounds bounds, Vector2f point) noexcept;
};

}
