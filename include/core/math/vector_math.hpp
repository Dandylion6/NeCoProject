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
	return {std::roundf(vector.x), std::roundf(vector.y)};
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
		Math::Remap(fromMin.x, fromMax.x, toMin.x, toMax.x, value.x),
		Math::Remap(fromMin.y, fromMax.y, toMin.y, toMax.y, value.y)
	};
}


inline Vector2f Remap(const Vector2f from, const Vector2f to, const Vector2f position) noexcept
{
    return {
        Math::Remap(0.0f, from.x, 0.0f, to.x, position.x),
        Math::Remap(0.0f, from.y, 0.0f, to.y, position.y)
    };
}


/**
 * @brief Computes the component-wise inverse linear interpolation of a position between two points.
 *
 * For each axis (x and y) independently, calculates how far @p position lies between
 * @p from and @p to, expressed as a fraction. This is the inverse of Lerp: given a value
 * produced by lerping between @p from and @p to, this recovers the original interpolation factor.
 *
 * @param from     The reference point corresponding to a result of 0.0 on each axis.
 * @param to       The reference point corresponding to a result of 1.0 on each axis.
 * @param position The point to evaluate against the [from, to] range on each axis.
 *
 * @return A Vector2f where each component is the interpolation factor for that axis.
 *         Values outside [0, 1] mean @p position lies outside the [from, to] range on that axis.
 *
 * @note If @c from.x == @c to.x (or likewise for y), that component's result depends on how
 *       Math::InverseLerp handles division by zero.
 *
 * @see Math::InverseLerp
 */
inline Vector2f InverseLerp(const Vector2f from, const Vector2f to, const Vector2f position) noexcept
{
    return {
        Math::InverseLerp(from.x, to.x, position.x),
        Math::InverseLerp(from.y, to.y, position.y)
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
inline Vector2f Lerp(const Vector2f start, const Vector2f end, const float time) noexcept
{
	return {
		Nc::Math::Lerp(start.x, end.x, time),
		Nc::Math::Lerp(start.y, end.y, time)
	};
}


/**
 * @brief Smoothly approaches a target vector over time using exponential decay.
 *
 * Computes a frame-rate–independent interpolation between <c>start</c> and <c>end</c>
 * based on <c>deltaTime</c> and <c>speed</c>. Larger speed values increase responsiveness.
 *
 * This method is useful for smoothing abrupt changes such as camera motion,
 * UI transitions, or gradual parameter adjustments.
 *
 * @param start Current vector.
 * @param end Target vector.
 * @param deltaTime Elapsed time since the previous update.
 * @param speed Smoothing rate; higher values yield faster convergence.
 * @return The new interpolated value approaching end.
 */
inline Vector2f SmoothApproach(const Vector2f start, const Vector2f end, const float deltaTime, const float speed)
{
	return {
		Math::SmoothApproach(start.x, end.x, deltaTime, speed),
		Math::SmoothApproach(start.y, end.y, deltaTime, speed)
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
inline float MagnitudeOf(const Nc::Vector2f vector) noexcept { return std::sqrt(SqrMagnitudeOf(vector)); };


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
	return {a.x * b.x, a.y * b.y};
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
