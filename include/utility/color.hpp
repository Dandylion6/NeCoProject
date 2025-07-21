#pragma once
#include "raylib.h"
#include <cstdint>


namespace Nc
{
	struct Hex;

	struct RGBa
	{
		uint8_t red = 0xff;
		uint8_t green = 0xff;
		uint8_t blue = 0xff;
		uint8_t alpha = 0xff;

		constexpr RGBa() = default;
		constexpr RGBa(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			red = r, green = g, blue = b, alpha = a;
		};

		RGBa(uint32_t hex);
		RGBa(Color color);
		RGBa(Hex hex);

		static RGBa White() { return RGBa(255u, 255u, 255u, 255u); };

		void SetAlpha(float alpha);

		operator Color() const;
	};

	struct Hex
	{
		uint32_t hex = 0xffffffff;

		constexpr Hex() = default;
		constexpr Hex(int hex): hex(static_cast<uint32_t>(hex)) { };
		constexpr Hex(uint32_t hex): hex(hex) { };

		Hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		Hex(Color rgba);
		Hex(RGBa rgba);

		operator Color() const;
	};
}