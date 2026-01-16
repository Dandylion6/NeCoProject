#pragma once
#include <cmath>

#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"


namespace Nc::Vector
{
/**
 * @brief Rounds both components of a vector to the nearest integer.
 * 
 * @param vector Input vector.
 * @return Rounded vector.
 */
inline Vector2f Round(const Nc::Vector2f vector) noexcept
{
	return { std::roundf(vector.x), std::roundf(vector.y) };
}


/**
 * @brief Remaps a vector’s components from one range to another.
 * 
 * @param fromMin Minimum of the source range.
 * @param fromMax Maximum of the source range.
 * @param toMin Minimum of the destination range.
 * @param toMax Maximum of the destination range.
 * @param value Input vector.
 * @return Remapped vector.
 * 
 */
inline Vector2f Remap(
	const Vector2f fromMin,
	const Vector2f fromMax,
	const Vector2f toMin,
	const Vector2f toMax,
	const Vector2f value
) noexcept
{
	return {
		Math::Remap(fromMin.x, fromMax.x, toMin.x, toMax.y, value.x),
		Math::Remap(fromMin.y, fromMax.y, toMin.y, toMax.y, value.y)
	};
}


/** 
 * @brief Linearly interpolates between two vectors.
 * 
 * @param start Start vector.
 * @param end End vector.
 * @param time Interpolation factor in range [0, 1].
 * @return Interpolated vector.
*/
inline Vector2f Lerp(const Nc::Vector2f start, const Nc::Vector2f end, const float time) noexcept
{
	return {
		Nc::Math::Lerp(start.x, end.x, time),
		Nc::Math::Lerp(start.y, end.y, time)
	};
}


/**
 * @brief Returns the smaller of the two vector components.
 * 
 * @param vector Input vector.
 * @return Minimum component value.
 */
inline float MinOf(const Nc::Vector2f vector) noexcept { return std::fminf(vector.x, vector.y); };


/**
 * @brief Returns the larger of the two vector components.
 * 
 * @param vector Input vector.
 * @return Maximum component value.
 */
inline float MaxOf(const Nc::Vector2f vector) noexcept { return std::fmaxf(vector.x, vector.y); };


/**
 * @brief Returns the squared `magnitude` of the vector (faster than MagnitudeOf).
 * 
 * @param vector Input vector.
 * @return Squared magnitude of the vector.
 */
constexpr float SqrMagnitudeOf(const Nc::Vector2f vector) noexcept
{
	return vector.x * vector.x + vector.y * vector.y;
};


/**
 * @brief Returns the `magnitude` (length) of the vector.
 * 
 * @param vector Input vector.
 * @return Magnitude of the vector.
 */
inline float MagnitudeOf(const Nc::Vector2f vector) noexcept { return std::sqrtf(SqrMagnitudeOf(vector)); };


/**
 * @brief Calculates the `distance` between two vectors.
 * 
 * @param a First vector.
 * @param b Second vector.
 * @return Distance between the two.
 */
inline float DistanceBetween(const Nc::Vector2f a, const Nc::Vector2f b) noexcept { return MagnitudeOf(a - b); };


/**
 * @brief Calculates the squared `distance` between two vectors.
 * 
 * @param a First vector.
 * @param b Second vector.
 * @return Squared distance between the two.
 */
constexpr float SqrDistanceBetween(const Nc::Vector2f a, const Nc::Vector2f b) noexcept
{
	return SqrMagnitudeOf(a - b);
}


/**
 * @brief Returns a `normalized` copy of the vector (length = 1).
 * 
 * @param vector Input vector.
 * @return Normalized vector.
 */
inline Vector2f Normalized(const Nc::Vector2f vector) noexcept
{
	const float length = MagnitudeOf(vector);
	return Nc::Vector2f(vector.x, vector.y) / length;
}


/** 
 * @brief Performs a component-wise `modulation` of two vectors.
 * 
 * Each component of `a` get multiplied with the respective component of `b`.
 * 
 * @param a First vector.
 * @param b Second vector.
 * @return The modulated vector.
 */
constexpr Vector2f Modulate(const Nc::Vector2f a, const Nc::Vector2f b) noexcept
{
	return { a.x * b.x, a.y * b.y };
}


/**
 * @brief Calculates the dot product of the two vectors.
 * 
 * @param a First vector.
 * @param b Second vector.
 * @return Resulting dot product.
 */
inline float Dot(const Nc::Vector2f a, const Nc::Vector2f b) noexcept
{
	return a.x * b.x + a.y * b.y;
}
}
