#pragma once
#include "core/data/color.hpp"
#include "core/data/font.hpp"
#include "raylib.h"
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
/**
 * @brief Stores information to render text.
 * 
 * Has many variables that dictate how the text is shown.
 * 
 * Usage example:
 * 
 * ```cpp
 * registry.emplace<Component::UI::Transform>(entity, ...);
 * registry.emplace<Component::Text>(
 *	entity,
 *	"Hello, World!",
 *	BLACK,
 *	Nc::Font::WDXL,
 *	Nc::Font::Size::Huge,
 *	Alignment::Left,
 *	4u
 * );
 * ```
 */
struct Text final
{
	std::string text = "Hello, World!";
	Nc::RGBa color = RAYWHITE;
	Nc::Font::Style style = Nc::Font::WDXL;
	Nc::Font::Size fontSize = Nc::Font::Size::Medium;
	Alignment alignment = Alignment::Center;
	uint8_t spacing = 0u;


	// ------ Constructors ------

	constexpr Text() noexcept = default;
	constexpr Text(
		std::string&& text,
		Nc::RGBa color = RAYWHITE,
		Nc::Font::Style style = Nc::Font::WDXL,
		Nc::Font::Size fontSize = Nc::Font::Size::Medium,
		Alignment alignment = Alignment::Center,
		uint8_t spacing = 0u
	) :
		text(std::move(text)),
		color(color),
		style(style),
		fontSize(fontSize),
		alignment(alignment),
		spacing(spacing)
	{ };
};

}