#pragma once
#include "raylib.h"


enum Direction
{
	Up,
	Down,
	Left,
	Right,
};

namespace Nc
{
	struct Vector2f;


	struct alignas(8u) Vector2i
	{
		int x = 0, y = 0;

		constexpr Vector2i() = default;
		constexpr Vector2i(int x, int y): x(x), y(y) { };

		Vector2i(Vector2 vector);

		static Vector2i Zero();

		operator Vector2() const;
		operator Vector2f() const;

		Vector2i operator*(const float factor) const;
		Vector2i operator*(const int factor) const;
		Vector2i operator-(const Vector2i other) const;
	};


	struct alignas(8u) Vector2f
	{
		float x = 0.0f, y = 0.0f;

		constexpr Vector2f() = default;
		constexpr Vector2f(float x, float y): x(x), y(y) { };

		Vector2f(Vector2 vector);

		static Vector2f Zero();
		static Vector2f Up();
		static Vector2f Down();
		static Vector2f Right();
		static Vector2f Up(float length);
		static Vector2f Down(float length);
		static Vector2f Right(float length);
		static Vector2f Round(const Vector2f& vector);

		operator Vector2() const;
		operator Vector2i() const;

		Vector2f operator+(const Vector2f& other) const;
		Vector2f operator-(const Vector2f& other) const;
		Vector2f operator*(const Vector2f& other) const;
		Vector2f operator*(const float factor) const;
		Vector2f operator/(const float factor) const;
		void operator+=(const Vector2f& other);
		void operator-=(const Vector2f& other);
		void operator*=(const float factor);
		void operator/=(const float factor);
	};
}