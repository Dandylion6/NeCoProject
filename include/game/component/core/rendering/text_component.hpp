#pragma once
#include <cstdint>
#include <string>
#include <utility>

#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/font.hpp"


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
 * @code
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
 * @endcode
 */
struct Text final
{
	std::string text = "Hello, World!";
	Nc::RGBa color = Nc::RGBa(RAYWHITE);
	Nc::Font::Style style = Nc::Font::WDXL;
	Nc::Font::Size fontSize = Nc::Font::Size::Medium;
	Alignment alignment = Alignment::Center;
	uint8_t spacing = 0u;


	// ------ Constructors ------

	constexpr Text() noexcept = default;
	explicit constexpr Text(
		std::string&& text,
		const Nc::RGBa color = Nc::RGBa(RAYWHITE),
		const Nc::Font::Style style = Nc::Font::WDXL,
		const Nc::Font::Size fontSize = Nc::Font::Size::Medium,
		const Alignment alignment = Alignment::Center,
		const uint8_t spacing = 0u
	) :
		text(std::move(text)),
		color(color),
		style(style),
		fontSize(fontSize),
		alignment(alignment),
		spacing(spacing) { }

	explicit constexpr Text(
		std::string&& text,
		const Nc::Hex color = Nc::Hex(0xffffffff),
		const Nc::Font::Style style = Nc::Font::WDXL,
		const Nc::Font::Size fontSize = Nc::Font::Size::Medium,
		const Alignment alignment = Alignment::Center,
		const uint8_t spacing = 0u
	) :
		text(std::move(text)),
		color(color),
		style(style),
		fontSize(fontSize),
		alignment(alignment),
		spacing(spacing) { }
};

}