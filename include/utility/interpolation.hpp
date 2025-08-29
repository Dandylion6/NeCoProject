#pragma once
#include "raylib.h"
#include "utility/vector2.hpp"
#include <cmath>


namespace Math
{
	float Lerp(float a, float b, float t);
	float InverseLerp(float a, float b, float value);
	float Remap(Nc::Vector2f a, Nc::Vector2f b, float value);
	float SmoothApproach(float a, float b, float deltaTime, float speed);
	float SineInOut(float x);
	float QuadIn(float x);
	float QuadOut(float x);
	float CubicOut(float x);
	float ExpoIn(float x);
	float ExpoOut(float x);
	float BackOut(float x);
}