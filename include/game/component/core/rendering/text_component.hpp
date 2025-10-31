#pragma once
#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/font_style.hpp"
#include <cstdint>
#include <string>
#include <utility>


enum class FontSize: uint8_t
{
	Tiny = 16u,     // For very subtle details, debug info, or very minor secondary elements.
    Small = 24u,    // Good for general menu text, item descriptions in inventories, standard UI labels.
    Medium = 32u,   // A solid default for dialogue, main button text, and common informational displays.
    Large = 48u,    // Perfect for section titles, important prompts, or quest objectives.
    Huge = 64u,     // Use for major headings, "Game Over", or significant notifications.
    Giant = 80u,    // Big, bold titles for splash screens, main menu, or level complete.
    Massive = 96u,  // If you want to really make a statement on a title screen or a dramatic announcement!
};


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
		FontStyle style = WDXL;
		FontSize fontSize = FontSize::Medium;
		Alignment alignment = Alignment::Center;
		uint8_t spacing = 0u;

		Text() = default;
		Text(
			std::string&& text,
			Nc::RGBa color = RAYWHITE,
			FontStyle style = WDXL,
			FontSize fontSize = FontSize::Medium,
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