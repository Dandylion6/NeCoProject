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
    Tiny = 16u,
    Small = 24u,
    Medium = 32u,
    Large = 48u,
    Huge = 64u,
    Giant = 80u,
    Massive = 96u,
};


inline float SizeToFloat(const Size size) noexcept
{
    switch (size)
    {
    case Size::Small:
        return static_cast<float>(Nc::Font::Size::Small);
    case Size::Medium:
        return static_cast<float>(Nc::Font::Size::Medium);
    case Size::Large:
        return static_cast<float>(Nc::Font::Size::Large);
    case Size::Huge:
        return static_cast<float>(Nc::Font::Size::Huge);
    case Size::Giant:
        return static_cast<float>(Nc::Font::Size::Giant);
    case Size::Massive:
        return static_cast<float>(Nc::Font::Size::Massive);
    default: return static_cast<float>(Nc::Font::Size::Small);
    }
}

}


struct FontKey final
{
    // ------ Members ------

    Nc::Font::Style style = Nc::Font::WDXL;
    Nc::Font::Size fontSize = Nc::Font::Size::Tiny;


    // ------ Constructors ------

    constexpr FontKey(Nc::Font::Style style, Nc::Font::Size fontSize) noexcept
        : style(style), fontSize(fontSize)
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
        const size_t hash1 = std::hash<int>{ }(static_cast<int>(key.style));
        const size_t hash2 = std::hash<int>{ }(static_cast<int>(key.fontSize));
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }
};
