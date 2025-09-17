#pragma once
#include "raylib.h"
#include "utility/color.hpp"
#include <cstdint>
#include <string>
#include <utility>


enum FontStyle: uint8_t
{
	WDXL
};


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


struct FontKey
{
	FontStyle style = WDXL;
	uint8_t fontSize = 16u;

	FontKey(FontStyle style, uint8_t fontSize): style(style), fontSize(fontSize) { };
	bool operator==(const FontKey& other) const 
	{
        return style == other.style && fontSize == other.fontSize;
    }
};


struct FontKeyHash 
{
    size_t operator()(const FontKey& k) const 
	{
        size_t h1 = std::hash<int>{}(static_cast<int>(k.style));
        size_t h2 = std::hash<int>{}(k.fontSize);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2)); 
    }
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