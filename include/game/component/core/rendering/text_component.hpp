#pragma once
#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/font.hpp"
#include <cstdint>
#include <string>
#include <utility>


enum class Alignment: uint8_t
{
	Left,
	Center,
	Right,
	BottomLeft,
};


namespace Component
{
	struct Text
	{
		std::string text = "Hello, World!";
		Nc::RGBa color = RAYWHITE;
		Nc::Font::Style style = Nc::Font::WDXL;
		Nc::Font::Size fontSize = Nc::Font::Size::Medium;
		Alignment alignment = Alignment::Center;
		uint8_t spacing = 0u;

		Text() = default;
		Text(
			std::string&& text,
			Nc::RGBa color = RAYWHITE,
			Nc::Font::Style style = Nc::Font::WDXL,
			Nc::Font::Size fontSize = Nc::Font::Size::Medium,
			Alignment alignment = Alignment::Center,
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