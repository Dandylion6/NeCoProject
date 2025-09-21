#include "raylib.h"
#include "utility/interpolation.hpp"
#include <cmath>


float Math::Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float Math::InverseLerp(float a, float b, float value)
{
	if (a == b) return value <= a ? 0.0f : 1.0f;
	return (value - a) / (b - a);
}

float Math::Remap(Nc::Vector2f a, Nc::Vector2f b, float value)
{
	float factor = Math::InverseLerp(a.x, a.y, value);
	return Math::Lerp(b.x, b.y, factor);
}

float Math::SmoothApproach(float a, float b, float deltaTime, float speed)
{
	return a + (b - a) * (1.0f - std::expf(-speed * deltaTime));
}

float Math::SineIn(float x)
{
	return 1.0f - std::cosf((x * PI) * 0.5f);
}

float Math::SineInOut(float x)
{
	return -(std::cosf(PI * x) - 1.0f) * 0.5f;
}

float Math::QuadIn(float x)
{
	return x * x;
}

float Math::QuadOut(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x);
}

float Math::CubicOut(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x) * (1.0f - x);
}

float Math::ExpoIn(float x)
{
	return std::exp2f(10.0f * (x - 1.0f));
}

float Math::ExpoOut(float x)
{
	return x == 1.0f ? 1.0f : 1.0f - std::exp2f(-10.0f * x);
}

float Math::BackOut(float x)
{
	const float C1 = 1.70158f, C3 = C1 + 1.0f;
	return 1.0f + C3 * std::powf(x - 1.0f, 3.0f) + C1 * std::powf(x - 1.0f, 2.0f);
}