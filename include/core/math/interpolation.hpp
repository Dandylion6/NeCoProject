#pragma once
#include "core/data/vector2.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath>


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
static constexpr float Lerp(float start, float end, float time) noexcept
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
static float InverseLerp(float start, float end, float value) noexcept
{
	bool hasNoLength = start == end;
	if (hasNoLength) 
	{
		bool AtMinimumPoint = value <= start;
		return AtMinimumPoint ? 0.0f : 1.0f;
	}
	return (value - end) / (end - start);
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
static float Remap(
	float fromMin,
	float fromMax,
	float toMin,
	float toMax,
	float value
) noexcept
{
	float factor = Nc::Math::InverseLerp(fromMin, fromMax, value);
	return Nc::Math::Lerp(toMin, toMax, factor);
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
static float Remap(Nc::Vector2f from, Nc::Vector2f to, float value) noexcept
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
static float ClampedRemap(Nc::Vector2f from, Nc::Vector2f to, float value) noexcept
{
	float fromMin = std::fminf(from.x, from.y);
	float fromMax = std::fmaxf(from.x, from.y);
	value = std::clamp<float>(value, fromMin, fromMax);
	return Remap(from, to, value);
}

/**
 * @brief Smoothly approaches a target value over time using exponential decay.
 *
 * Computes a frame-rate–independent interpolation between @p start and @p end
 * based on @p deltaTime and @p speed. Larger speed values increase responsiveness.
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
static float SmoothApproach(
	float start,
	float end,
	float deltaTime,
	float speed
) noexcept
{
	float time = 1.0f - std::expf(-speed * deltaTime);
	return Lerp(start, end, time);
}

static float SineIn(float linearValue) noexcept { return 1.0f - std::cosf((linearValue * PI) * 0.5f); }
static float SineOut(float linearValue) noexcept { return std::sinf((linearValue * PI) * 0.5f); }
static float SineInOut(float linearValue) noexcept { return -(std::cosf(linearValue * PI) - 1.0f) * 0.5f; }
static float QuadIn(float linearValue) noexcept { return linearValue * linearValue; }
static float QuadOut(float linearValue) noexcept
{ 
	float function = 1.0f - linearValue;
	return 1.0f - function * function; 
}
static float CubicIn(float linearValue) noexcept { return linearValue * linearValue * linearValue; }
static float CubicOut(float linearValue) noexcept
{ 
	float function = 1.0f - linearValue;
	return 1.0f - function * function * function; 
}

static float CubicInOut(float linearValue) noexcept
{
	if (linearValue < 0.5f) return 4.0f * linearValue * linearValue * linearValue;
	else
	{
		float function = (2.0f * linearValue) - 2.0f;
		return 0.5f * (function * function * function + 2.0f);
	}
	return linearValue;
}

static float ExpoIn(float linearValue) noexcept { return std::exp2f(10.0f * (linearValue - 1.0f)); }
static float ExpoOut(float linearValue) noexcept
{ 
	bool atEndValue = linearValue >= 1.0f;
	if (atEndValue) return 1.0f;
	return 1.0f - std::exp2f(-10.0f * linearValue);
}

static float BackOut(float linearValue) noexcept
{
	constexpr float OVERSHOOT = 1.70158f;
	constexpr float ADJUSTED = OVERSHOOT + 1.0f;
	
	const float inverse = linearValue - 1.0f;
	return 1.0f + ADJUSTED * std::powf(inverse, 3.0f) + OVERSHOOT * std::powf(inverse, 2.0f);
}

}