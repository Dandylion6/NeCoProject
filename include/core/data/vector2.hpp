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

struct Vector2i final
{
	// ------ Members ------

	int x = 0, y = 0;


	// ------ Constructors ------

	constexpr Vector2i() = default;
	constexpr Vector2i(int x, int y) : x(x), y(y) { };
	constexpr Vector2i(
		Vector2 vector
	) noexcept :
		x(static_cast<int>(vector.x)), 
		y(static_cast<int>(vector.y))
	{ };


	// ------ Conversion ------

	constexpr operator Vector2() const noexcept { return { static_cast<float>(x), static_cast<float>(y) }; };


	// ------ Utility ------

	/// @brief Returns a zero vector (0, 0).
	constexpr static Vector2i Zero() noexcept { return Vector2i(0, 0); };


	// ------ Operations ------

	constexpr Vector2i operator-(const Vector2i other) const { return Vector2i(x - other.x, y - other.y); };
	constexpr Vector2i operator*(const int factor) const { return Vector2i(x * factor, y * factor); };
	constexpr Vector2i operator*(const float factor) const 
	{ 
		return Vector2i(static_cast<int>(x * factor), static_cast<int>(y * factor)); 
	};
};

}


namespace Nc
{

struct Vector2f final
{
	// ------ Members ------

	float x = 0.0f, y = 0.0f;


	// ------ Constructors ------

	constexpr Vector2f() = default;
	constexpr Vector2f(float x, float y) noexcept : x(x), y(y) { };
	constexpr Vector2f(int x, int y) noexcept : 
		x(static_cast<float>(x)), y(static_cast<float>(y)) 
	{ };
	
	constexpr Vector2f(
		Vector2 vector
	) noexcept :
		x(vector.x), y(vector.y)
	{ };


	// ------ Conversion ------
	
	constexpr operator Vector2() const noexcept { return { x, y }; }
	constexpr operator Vector2i() const noexcept { return Vector2i(static_cast<int>(x), static_cast<int>(y)); };


	// ------ Utility ------

	/// @brief Returns a zero vector (0.0f, 0.0f).
	constexpr static Vector2f Zero() noexcept { return Vector2f(0.0f, 0.0f); };
	/// @brief Returns an upward unit vector (0.0f, 1.0f).
	constexpr static Vector2f Up() noexcept { return Vector2f(0.0f, 1.0f); };
	/// @brief Returns a downward unit vector (0.0f, -1.0f).
	constexpr static Vector2f Down() noexcept { return Vector2f(0.0f, -1.0f); };
	/// @brief Returns a rightward unit vector (1.0f, 0.0f).
	constexpr static Vector2f Right() noexcept { return Vector2f(1.0f, 0.0f); };
	/**
	 * @brief Returns an upward vector with a given length.
	 * 
	 * @param length Desired vector length.
	 */
	constexpr static Vector2f Up(const float length) noexcept { return Vector2f(0.0f, length); };
	/**
	 * @brief Returns a downward vector with a given length.
	 * 
	 * @param length Desired vector length.
	 */
	constexpr static Vector2f Down(const float length) noexcept { return Vector2f(0.0f, -length); };
	/**
	 * @brief Returns a rightward vector with a given length.
	 * 
	 * @param length Desired vector length.
	 */
	constexpr static Vector2f Right(const float length) noexcept { return Vector2f(length, 0.0f); };
	/**
	 * @brief Returns a leftward vector with a given length.
	 * 
	 * @param length Desired vector length.
	 */
	constexpr static Vector2f Left(const float length) noexcept { return Vector2f(-length, 0.0f); };
	/**
	 * @brief Returns a uniform scale vector where x = y = scale.
	 * 
	 * @param scale Scale factor.
	 */
	constexpr static Vector2f Scale(const float scale) noexcept { return Vector2f(scale, scale); };


	// ------ Operations ------

	constexpr Vector2f operator-() const { return Vector2f(-x, -y); };
	constexpr Vector2f operator+(const Vector2f other) const { return Vector2f(x + other.x, y + other.y); };
	constexpr Vector2f operator-(const Vector2f other) const { return Vector2f(x - other.x, y - other.y); };
	constexpr Vector2f operator*(const Vector2f other) const { return Vector2f(x * other.x, y * other.y); };
	constexpr Vector2f operator*(const float factor) const  { return Vector2f(x * factor, y * factor); };
	constexpr Vector2f operator/(const Vector2f other) const { return Vector2f(x / other.x, y / other.y); };
	constexpr Vector2f operator/(const float factor) const { return Vector2f(x / factor, y / factor); };
	constexpr void operator+=(const Vector2f other) noexcept { x += other.x, y += other.y; };
	constexpr void operator-=(const Vector2f other) noexcept { x -= other.x, y -= other.y; };
	constexpr void operator*=(const float factor) noexcept { x *= factor, y *= factor; };
	constexpr void operator/=(const float factor) noexcept { x /= factor, y /= factor; };
	constexpr bool operator==(const Vector2f other) const noexcept { return x == other.x && y == other.y; };
};

}