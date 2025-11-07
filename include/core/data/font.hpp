#pragma once
#include <cstdint>
#include <functional>


namespace Nc::Font
{
    enum Style : uint8_t
    {
        WDXL
    };

    enum class Size : uint8_t
    {
        Tiny = 16u,     // For very subtle details, debug info, or very minor secondary elements.
        Small = 24u,    // Good for general menu text, item descriptions in inventories, standard UI labels.
        Medium = 32u,   // A solid default for dialogue, main button text, and common informational displays.
        Large = 48u,    // Perfect for section titles, important prompts, or quest objectives.
        Huge = 64u,     // Use for major headings, "Game Over", or significant notifications.
        Giant = 80u,    // Big, bold titles for splash screens, main menu, or level complete.
        Massive = 96u,  // If you want to really make a statement on a title screen or a dramatic announcement!
    };
}


struct FontKey final
{
    // ------ Members ------

	Nc::Font::Style style = Nc::Font::WDXL;
	Nc::Font::Size fontSize = Nc::Font::Size::Tiny;


    // ------ Constructors ------

	constexpr FontKey(
        Nc::Font::Style style, 
        Nc::Font::Size fontSize
    ) noexcept : 
        style(style), 
        fontSize(fontSize) 
    { };


    // ------ Operations ------

	bool operator==(const FontKey& other) const
	{
        return style == other.style && fontSize == other.fontSize;
    }
};


struct FontKeyHash final
{
    // ------ Operations ------

    size_t operator()(const FontKey& key) const 
	{
        size_t hash1 = std::hash<int>{ }(static_cast<int>(key.style));
        size_t hash2 = std::hash<int>{ }(static_cast<int>(key.fontSize));
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2)); 
    }
};