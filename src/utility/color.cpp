#include "raylib.h"
#include "utility/color.hpp"
#include <cstdint>


namespace Nc
{
#pragma region RGBa

	RGBa::RGBa(uint32_t hex)
	{
		red = (hex & 0xff000000) >> 24u;
		green = (hex & 0x00ff0000) >> 16u;
		blue = (hex & 0x0000ff00) >> 8u;
		alpha = hex & 0x000000ff;
	}


	RGBa::RGBa(Color color)
	{
		red = color.r, green = color.g, blue = color.b, alpha = color.a;
	}

	RGBa::RGBa(Hex hex)
	{
		red = (hex.hex & 0xff000000) >> 24u;
		green = (hex.hex & 0x00ff0000) >> 16u;
		blue = (hex.hex & 0x0000ff00) >> 8u;
		alpha = hex.hex & 0x000000ff;
	}


	Vector4 RGBa::ToFloat() const
	{
		return Vector4(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
	}


	/// @brief Sets the alpha channel of the color.
	/// @details This method takes a float value between 0.0 and 1.0 and converts it to an 8-bit unsigned integer (uint8_t)
	/// to represent the alpha channel. A value of 0.0 corresponds to fully transparent, and 1.0 corresponds to fully opaque.
	/// The conversion is done by multiplying the float by 255 and casting the result to a uint8_t.
	/// @param alpha A float representing the desired transparency, where 0.0 is transparent and 1.0 is opaque.
	void RGBa::SetAlpha(float alpha)
	{
		this->alpha = static_cast<uint8_t>(alpha * 255.0f);
	}


	RGBa::operator Color() const
	{
		Color color { red, green, blue, alpha };
		return color;
	}

#pragma endregion

#pragma region Hex

	Hex::Hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		hex = (r << 24u) | (g << 16u) | (b << 8u) | a;
	}

	Hex::Hex(Color rgba): Hex(rgba.r, rgba.g, rgba.b, rgba.a) { };
	
	Hex::Hex(RGBa rgba): Hex(static_cast<Color>(rgba)) { }
	
	Hex::operator Color() const
	{
		Color color = RGBa(hex);
		return color;
	}

#pragma endregion
}
