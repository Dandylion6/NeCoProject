#pragma once
#include <cstdint>
#include <functional>


enum FontStyle: uint8_t
{
	WDXL
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