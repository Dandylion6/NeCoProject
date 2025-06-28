#pragma once
#include "raylib.h"
#include "utility/color.h"
#include "utility/vector2.h"


struct RenderContext
{
	static constexpr Nc::Vector2i DISPLAY_SIZE = Nc::Vector2i(980, 740);
	static constexpr Nc::Vector2i RADAR_SIZE = Nc::Vector2i(320, 320);
	static constexpr Nc::Vector2f RADAR_POSITION = Nc::Vector2f(282.0f, 100.0f);
	static constexpr Nc::Hex BACKGROUND_COLOR = 0x07090dff;

	RenderTexture2D renderTexture { };
	RenderTexture2D radarRenderTexture { };
	Rectangle renderRectangle { };
	float renderScale = 1.0f;
	Nc::Vector2i windowSize { };
};