#include "raylib.h"
#include "utility/vector2.h"
#include <cmath>


namespace Nc
{
	Vector2i::Vector2i(Vector2 vector)
	{
		x = static_cast<int>(vector.x), y = static_cast<int>(vector.y);
	}


	Vector2i Vector2i::Zero()
	{
		return Vector2i(0, 0);
	}


	Vector2i::operator Vector2() const
	{
		return Vector2 { static_cast<float>(x), static_cast<float>(y) };
	}


	Vector2i::operator Vector2f() const
	{
		return Vector2f(static_cast<float>(x), static_cast<float>(y));
	}


	Vector2i Vector2i::operator*(const float factor) const
	{
		return Vector2i(static_cast<int>(x * factor), static_cast<int>(y * factor));
	}


	Vector2i Vector2i::operator*(const int factor) const
	{
		return Vector2i(x * factor, y * factor);
	}


	Vector2i Vector2i::operator-(const Vector2i other) const
	{
		return Vector2i(x - other.x, y - other.y);
	}


	Vector2f::Vector2f(Vector2 vector)
	{
		x = vector.x, y = vector.y;
	}


	Vector2f Vector2f::Zero()
	{
		return Vector2f(0.0f, 0.0f);
	}


	Vector2f Vector2f::Up()
	{
		return Vector2f(0.0f, 1.0f);
	}


	Vector2f Vector2f::Down()
	{
		return Vector2f(0.0f, -1.0f);
	}

	Vector2f Vector2f::Right()
	{
		return Vector2f(1.0f, 0.0f);
	}


	Vector2f Vector2f::Up(float length)
	{
		return Vector2f(0.0f, length);
	}


	Vector2f Vector2f::Down(float length)
	{
		return Vector2f(0.0f, -length);
	}


	Vector2f Vector2f::Right(float length)
	{
		return Vector2f(length, 0.0f);
	}


	Vector2f Vector2f::Scale(float scale)
	{
		return Vector2f(scale, scale);
	}


	Vector2f Vector2f::Round(const Vector2f& vector)
	{
		return Vector2f(std::roundf(vector.x), std::roundf(vector.y));
	}


	float Vector2f::GetDistance() const
	{
		return std::sqrtf(GetSqrDistance());
	}


	float Vector2f::GetSqrDistance() const
	{
		return x * x + y * y;
	}


	float Vector2f::GetMin() const
	{
		return std::fminf(x, y);
	}


	Vector2f::operator Vector2() const
	{
		return Vector2 { x, y };
	}


	Vector2f::operator Vector2i() const
	{
		return Vector2i(static_cast<int>(x), static_cast<int>(y));
	}


	Vector2f Vector2f::operator-() const
	{
		return Vector2f(-x, -y);
	}


	Vector2f Vector2f::operator+(const Vector2f& other) const
	{
		return Vector2f(x + other.x, y + other.y);
	}


	Vector2f Vector2f::operator-(const Vector2f& other) const
	{
		return Vector2f(x - other.x, y - other.y);
	}


	Vector2f Vector2f::operator*(const Vector2f& other) const
	{
		return Vector2f(x * other.x, y * other.y);
	}


	Vector2f Vector2f::operator/(const Vector2f& other) const
	{
		return Vector2f(x / other.x, y / other.y);
	}


	Vector2f Vector2f::operator*(const float factor) const
	{
		return Vector2f(x * factor, y * factor);
	}


	Vector2f Vector2f::operator/(const float factor) const
	{
		return Vector2f(x / factor, y / factor);
	}


	void Vector2f::operator+=(const Vector2f& other)
	{
		x += other.x, y += other.y;
	}


	void Vector2f::operator-=(const Vector2f& other)
	{
		x -= other.x, y -= other.y;
	}


	void Vector2f::operator*=(const float factor)
	{
		x *= factor, y *= factor;
	}


	void Vector2f::operator/=(const float factor)
	{
		x /= factor, y /= factor;
	}


	bool Vector2f::operator==(const Vector2f other) const
	{
		return x == other.x && y == other.y;
	}
}