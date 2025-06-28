#pragma once
#include "raylib.h"
#include "utility/color.h"
#include <cstdint>
#include <string>
#include <utility>


enum FontStyle: uint8_t
{
	WDXL
};


namespace Component
{
	struct Text
	{
		enum Alignment: uint8_t
		{
			Left,
			Center,
			Right
		};

		std::string text = "Hello, World!";
		Nc::RGBa color = RAYWHITE;
		FontStyle style = WDXL;
		uint8_t fontSize = 8u;
		Alignment alignment = Center;
		uint8_t spacing = 0u;

		Text() = default;
		Text(
			std::string&& text,
			Nc::RGBa color = RAYWHITE,
			FontStyle style = WDXL,
			uint8_t fontSize = 8u,
			Alignment alignment = Center,
			uint8_t spacing = 0u
		):
			text(std::move(text)),
			color(color),
			style(style),
			fontSize(fontSize),
			alignment(alignment),
			spacing(spacing)
		{ };
	};
}