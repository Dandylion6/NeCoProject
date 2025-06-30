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


	struct Vector2i
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


	struct Vector2f
	{
		float x = 0.0f, y = 0.0f;

		constexpr Vector2f() = default;
		constexpr Vector2f(float x, float y): x(x), y(y) { };
		constexpr Vector2f(Vector2 vector) { x = vector.x, y = vector.y; };

		constexpr static Vector2f Zero() { return Vector2f(0.0f, 0.0f); };
		constexpr static Vector2f Up() { return Vector2f(0.0f, 1.0f); };
		constexpr static Vector2f Down() { return Vector2f(0.0f, -1.0f); };
		constexpr static Vector2f Right() { return Vector2f(1.0f, 0.0f); };
		constexpr static Vector2f Up(const float length) { return Vector2f(0.0f, length); };
		constexpr static Vector2f Down(const float length) { return Vector2f(0.0f, -length); };
		constexpr static Vector2f Right(const float length) { return Vector2f(length, 0.0f); };
		constexpr static Vector2f Scale(const float scale) { return Vector2f(scale, scale); };
		static Vector2f Round(const Vector2f& vector);

		float GetDistance() const;
		float GetSqrDistance() const;
		float GetMin() const;

		operator Vector2() const;
		operator Vector2i() const;

		Vector2f operator-() const;
		Vector2f operator+(const Vector2f& other) const;
		Vector2f operator-(const Vector2f& other) const;
		Vector2f operator*(const Vector2f& other) const;
		Vector2f operator/(const Vector2f& other) const;
		Vector2f operator*(const float factor) const;
		Vector2f operator/(const float factor) const;
		void operator+=(const Vector2f& other);
		void operator-=(const Vector2f& other);
		void operator*=(const float factor);
		void operator/=(const float factor);
		bool operator==(const Vector2f other) const;
	};
}