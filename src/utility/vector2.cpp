#include "raylib.h"
#include "utility/bounds.hpp"
#include "utility/interpolation.hpp"
#include "utility/vector2.hpp"
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


	Vector2f Vector2f::Round(const Vector2f& vector)
	{
		return Vector2f(std::roundf(vector.x), std::roundf(vector.y));
	}


	Vector2f Vector2f::Remap(Bounds from, Bounds to, const Vector2f value)
	{
		from = Bounds(Vector2f(from.min.x, from.max.x), Vector2f(from.min.y, from.max.y));
		to = Bounds(Vector2f(to.min.x, to.max.x), Vector2f(to.min.y, to.max.y));
		return Nc::Vector2f(Math::Remap(from.min, to.min, value.x), Math::Remap(from.max, to.max, value.y));
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


	Nc::Vector2f Vector2f::Normalized() const
	{
		float length = GetDistance();
		return Nc::Vector2f(x, y) / length;
	}


	void Vector2f::Normalize()
	{
		float length = GetDistance();
		x /= length, y /= length;
	}


	Vector2f::operator Vector2() const
	{
		return Vector2 { x, y };
	}


	Vector2f::operator Vector2i() const
	{
		return Vector2i(static_cast<int>(x), static_cast<int>(y));
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