#pragma once
#include <algorithm>
#include <cmath>

#include "raylib.h"
#include "core/data/vector2.hpp"


namespace Nc::Math
{
/**
 * @brief Linearly interpolates between two values.
 *
 * Computes the value between @p start and @p end at normalized position @p time.
 * A time of 0.0 returns start; 1.0 returns end.
 *
 * @param start The starting value.
 * @param end The ending value.
 * @param time Normalized interpolation factor in [0.0, 1.0].
 * @return Interpolated value between start and end.
 */
constexpr float Lerp(const float start, const float end, const float time) noexcept
{
	return start + (end - start) * time;
}


/**
 * @brief Calculates the normalized position of a value between two bounds.
 *
 * This performs the inverse of Lerp(): it returns how far @p value lies between
 * @p start and @p end, producing a normalized factor in [0.0, 1.0].
 *
 * When start and end are equal, the function returns 0.0 if the value is
 * below or equal to start, and 1.0 otherwise.
 *
 * @param start The lower bound of the range.
 * @param end The upper bound of the range.
 * @param value The input value.
 * @return Normalized factor representing value's position between start and end.
 */
inline float InverseLerp(const float start, const float end, const float value) noexcept
{
	if (start == end)
	{
		const bool atMinimumPoint = value <= start;
		return atMinimumPoint ? 0.0f : 1.0f;
	}
	return (value - start) / (end - start);
}


/**
 * @brief Remaps a value from one numeric range to another.
 *
 * Converts @p value from the source range defined by @p from into
 * the target range defined by @p to, preserving its relative position.
 *
 * @param fromMin Input minimum.
 * @param fromMax Input maximum.
 * @param toMin Output minimum.
 * @param toMax Output maximum.
 * @param value Input value within the source range.
 * @return Remapped value in the target range.
 */
inline float Remap(
	const float fromMin,
	const float fromMax,
	const float toMin,
	const float toMax,
	const float value
) noexcept
{
	const float factor = InverseLerp(fromMin, fromMax, value);
	return Lerp(toMin, toMax, factor);
}


/**
 * @brief Remaps a value from one numeric range to another.
 *
 * Converts @p value from the source range defined by @p from into
 * the target range defined by @p to, preserving its relative position.
 *
 * @param from Input range (x = min, y = max).
 * @param to Output range (x = min, y = max).
 * @param value Input value within the source range.
 * @return Remapped value in the target range.
 */
inline float Remap(const Vector2f from, const Vector2f to, const float value) noexcept
{
	return Remap(from.x, from.y, to.x, to.y, value);
}


/**
 * @brief Remaps a value between ranges, clamping it to the input range first.
 *
 * Ensures @p value remains within @p from before remapping. Useful when
 * dealing with untrusted or noisy input that may exceed bounds.
 *
 * @param from Input range (x = min, y = max).
 * @param to Output range (x = min, y = max).
 * @param value Input value to remap.
 * @return Remapped and clamped value.
 */
inline float ClampedRemap(const Vector2f from, const Vector2f to, float value) noexcept
{
	const float fromMin = std::fminf(from.x, from.y);
	const float fromMax = std::fmaxf(from.x, from.y);

	value = std::clamp<float>(value, fromMin, fromMax);
	return Remap(from, to, value);
}


/**
 * @brief Smoothly approaches a target value over time using exponential decay.
 *
 * Computes a frame-rate–independent interpolation between <c>start</c> and <c>end</c>
 * based on <c>deltaTime</c> and <c>speed</c>. Larger speed values increase responsiveness.
 *
 * This method is useful for smoothing abrupt changes such as camera motion,
 * UI transitions, or gradual parameter adjustments.
 *
 * @param start Current value.
 * @param end Target value.
 * @param deltaTime Elapsed time since the previous update.
 * @param speed Smoothing rate; higher values yield faster convergence.
 * @return The new interpolated value approaching end.
 */
inline float SmoothApproach(const float start, const float end, const float deltaTime, const float speed) noexcept
{
	const float time = 1.0f - std::exp(-speed * deltaTime);
	return Lerp(start, end, time);
}


inline float SineIn(const float linearValue) noexcept { return 1.0f - std::cos(linearValue * PI * 0.5f); }
inline float SineOut(const float linearValue) noexcept { return std::sin(linearValue * PI * 0.5f); }
inline float SineInOut(const float linearValue) noexcept { return -(std::cos(linearValue * PI) - 1.0f) * 0.5f; }
inline float QuadIn(const float linearValue) noexcept { return linearValue * linearValue; }


inline float QuadOut(const float linearValue) noexcept
{
	const float function = 1.0f - linearValue;
	return 1.0f - function * function;
}


inline float CubicIn(const float linearValue) noexcept { return linearValue * linearValue * linearValue; }


inline float CubicOut(const float linearValue) noexcept
{
	const float function = 1.0f - linearValue;
	return 1.0f - function * function * function;
}


inline float CubicInOut(const float linearValue) noexcept
{
	if (linearValue < 0.5f) return 4.0f * linearValue * linearValue * linearValue;

	const float function = (2.0f * linearValue) - 2.0f;
	return 0.5f * (function * function * function + 2.0f);
}


inline float ExpoIn(const float linearValue) noexcept { return std::exp2f(10.0f * (linearValue - 1.0f)); }


inline float ExpoOut(const float linearValue) noexcept
{
	if (linearValue >= 1.0f) return 1.0f;
	return 1.0f - std::exp2f(-10.0f * linearValue);
}


inline float BackOut(const float linearValue) noexcept
{
	constexpr float OVERSHOOT = 1.70158f;
	constexpr float ADJUSTED = OVERSHOOT + 1.0f;

	const float inverse = linearValue - 1.0f;
	return 1.0f + ADJUSTED * std::pow(inverse, 3.0f) + OVERSHOOT * std::pow(inverse, 2.0f);
}
}
