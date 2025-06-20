#pragma once
#include "raylib.h"
#include <cmath>


namespace Math
{
	float Lerp(float a, float b, float t);
	float SmoothApproach(float a, float b, float deltaTime, float speed);
	float SineInOut(float x);
	float QuadIn(float x);
	float QuadOut(float x);
	float CubicOut(float x);
	float ExpoIn(float x);
	float ExpoOut(float x);
	float BackOut(float x);
}