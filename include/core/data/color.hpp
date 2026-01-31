#pragma once
#include <cmath>
#include <cstdint>

#include "raylib.h"


namespace Nc
{
/**
 * @brief Represents a color with 8-bit red, green, blue, and alpha channels.
 * 
 * This structure provides utility for converting between Raylib's Color type,
 * 32-bit hexadecimal color values, and normalized float-based representations.
 * 
 * The channel order is (R, G, B, A), each stored as an unsigned 8-bit integer
 * in the range [0, 255].
 */
struct RGBa final
{
	// ------ Members ------

	uint8_t red = 0xff;
	uint8_t green = 0xff;
	uint8_t blue = 0xff;
	uint8_t alpha = 0xff;


	// ------ Constructors ------

	constexpr RGBa() = default;
	constexpr RGBa(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) noexcept
		: red(r), green(g), blue(b), alpha(a) { }

	explicit constexpr RGBa(const Color color) noexcept
		: red(color.r), green(color.g), blue(color.b), alpha(color.a) { }

	explicit constexpr RGBa(const uint32_t hex) noexcept
	{
		red = (hex & 0xff000000) >> 24u;
		green = (hex & 0x00ff0000) >> 16u;
		blue = (hex & 0x0000ff00) >> 8u;
		alpha = hex & 0x000000ff;
	}


	// ------ Conversion ------

	explicit constexpr operator Color() const noexcept { return { red, green, blue, alpha }; }
	explicit constexpr operator Vector4() const noexcept { return FloatFrom(*this); }


	// ------ Utility ------

	/**
	 * @brief Converts 8-bit RGBA values to a normalized float Vector4.
	 * 
	 * Each color channel in the input is divided by 255.0f, producing values
	 * in the range [0.0, 1.0]. This is often used when passing color data
	 * to shaders or APIs expecting normalized floats.
	 * 
	 * @param rgba is the source RGBa color (8-bit per channel).
	 * @return Vector4 containing normalized RGBA values.
	 */
	static constexpr Vector4 FloatFrom(const RGBa rgba) noexcept
	{
		const auto red = static_cast<float>(rgba.red) / 255.0f;
		const auto green = static_cast<float>(rgba.green) / 255.0f;
		const auto blue = static_cast<float>(rgba.blue) / 255.0f;
		const auto alpha = static_cast<float>(rgba.alpha) / 255.0f;
		return { red, green, blue, alpha };
	}


	/**
	 * @brief Sets the alpha channel of an RGBA color from a normalized float value.
	 * 
	 * Takes a float in the range [0.0, 1.0] and converts it to an 8-bit
	 * alpha channel by multiplying by 255 and rounding to the nearest integer.
	 * 
	 * @param rgba  Reference to the color to modify.
	 * @param alpha Normalized alpha in [0.0, 1.0], where 0.0 is transparent and 1.0 is fully opaque.
	 */
	static void SetAlphaFor(RGBa& rgba, float alpha) noexcept
	{
		alpha = std::roundf(alpha * 255.0f);
		rgba.alpha = static_cast<uint8_t>(alpha);
	}
};

}


namespace Nc
{
/**
 * @brief Helper for packing and unpacking color values to and from 32-bit hexadecimal format.
 * 
 * The layout follows 0xRRGGBBAA (red in the highest byte, alpha in the lowest).
 * These functions are primarily used for serialization, debugging, and asset I/O.
 */
struct Hex final
{
	// ------ Members ------

	uint32_t hex = 0xffffffff;


	// ------ Constructors ------

	constexpr Hex() noexcept = default;
	explicit constexpr Hex(const int hex) noexcept : hex(static_cast<uint32_t>(hex)) { };
	explicit constexpr Hex(const uint32_t hex) noexcept : hex(hex) { };
	explicit constexpr Hex(const Color rgba) noexcept : Hex(rgba.r, rgba.g, rgba.b, rgba.a) { };
	explicit constexpr Hex(const RGBa rgba) noexcept : Hex(static_cast<Color>(rgba)) { };

	constexpr Hex(
		const uint8_t r,
		const uint8_t g,
		const uint8_t b,
		const uint8_t a
	) noexcept
	{
		hex = (r << 24u) | (g << 16u) | (b << 8u) | a;
	}


	// ------ Conversion ------

	explicit constexpr operator RGBa() const noexcept { return RGBa(hex); }
	explicit constexpr operator Color() const noexcept
	{
		const auto color = RGBa(hex);
		return { color.red, color.green, color.blue, color.alpha };
	}
};

}
