#pragma once
#include "core/math/interpolation.hpp"
#include "core/data/vector2.hpp"
#include <cmath>


namespace Nc::Vector
{

/**
 * @brief Rounds both components of a vector to the nearest integer.
 * 
 * @param vector Input vector.
 * @return Rounded vector.
 */
static Nc::Vector2f Round(const Nc::Vector2f vector) noexcept
{
	return Nc::Vector2f(std::roundf(vector.x), std::roundf(vector.y));
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
static Nc::Vector2f Remap(
	const Nc::Vector2f fromMin,
	const Nc::Vector2f fromMax,
	const Nc::Vector2f toMin,
	const Nc::Vector2f toMax, 
	const Nc::Vector2f value
) noexcept
{
	float x = Nc::Math::Remap(fromMin.x, fromMax.x, toMin.x, toMax.y, value.x);
	float y = Nc::Math::Remap(fromMin.y, fromMax.y, toMin.y, toMax.y, value.y);
	return Nc::Vector2f(x, y);
}


/** 
 * @brief Linearly interpolates between two vectors.
 * 
 * @param start Start vector.
 * @param end End vector.
 * @param time Interpolation factor in range [0, 1].
 * @return Interpolated vector.
*/
constexpr static Nc::Vector2f Lerp(
	const Nc::Vector2f start,
	const Nc::Vector2f end,
	const float time
) noexcept
{
	return Vector2f(
		Nc::Math::Lerp(start.x, end.x, time), 
		Nc::Math::Lerp(start.y, end.y, time)
	);
}
	

/**
 * @brief Returns the smaller of the two vector components.
 * 
 * @param vector Input vector.
 * @return Minimum component value.
 */
static float MinOf(const Nc::Vector2f vector) noexcept { return std::fminf(vector.x, vector.y); };


/**
 * @brief Returns the larger of the two vector components.
 * 
 * @param vector Input vector.
 * @return Maximum component value.
 */
static float MaxOf(const Nc::Vector2f vector) noexcept { return std::fmaxf(vector.x, vector.y); };


/**
 * @brief Returns the squared magnitude of the vector (faster than MagnitudeOf).
 * 
 * @param vector Input vector.
 * @return Squared magnitude of the vector.
 */
static constexpr float SqrMagnitudeOf(const Nc::Vector2f vector) noexcept 
{
	return vector.x * vector.x + vector.y * vector.y; 
};


/**
 * @brief Returns the magnitude (length) of the vector.
 * 
 * @param vector Input vector.
 * @return Magnitude of the vector.
 */
static float MagnitudeOf(const Nc::Vector2f vector) noexcept { return std::sqrtf(SqrMagnitudeOf(vector)); };


/**
 * @brief Calculates the distance between two vectors.
 * 
 * @param vector1 First vector.
 * @param vector2 Second vector.
 * @return Distance between the two.
 */
static float DistanceOf(const Nc::Vector2f vector1, const Nc::Vector2f vector2) noexcept
{
	return MagnitudeOf(vector1 - vector2);
}


/**
 * @brief Calculates the squared distance between two vectors.
 * 
 * @param vector1 First vector.
 * @param vector2 Second vector.
 * @return Squared distance between the two.
 */
static constexpr float SqrDistanceOf(const Nc::Vector2f vector1, const Nc::Vector2f vector2) noexcept
{
	return SqrMagnitudeOf(vector1 - vector2);
}


/**
 * Returns a normalized copy of the vector (length = 1).
 * 
 * @param vector Input vector.
 * @return Normalized vector.
 */
static Nc::Vector2f Normalized(const Nc::Vector2f vector) noexcept
{
	float length = MagnitudeOf(vector);
	return Nc::Vector2f(vector.x, vector.y) / length;
}

}