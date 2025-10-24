#pragma once
#include "raylib.h"
#include "utility/bounds.hpp"
#include "utility/vector2.hpp"


struct LightingContext
{
	int textureLocation;
	int lightSourceCount;
	int lightPositionLocation;
	int lightColorLocation;
	int lightRangeLocation;
	int lightStrengthLocation;
	int lightTypeLocation;
	int lightDirectionLocation;
	int lightAngleLocation;
};


struct RenderContext
{
	static constexpr Nc::Vector2i DISPLAY_SIZE = Nc::Vector2i(980, 740);
	static constexpr Nc::Bounds RADAR_BOUNDS = Nc::Bounds(0.0f, 0.0f, 320.0f, 320.0f);
	static constexpr Nc::Vector2f RADAR_POSITION = Nc::Vector2f(282.0f, 100.0f);

	RenderTexture2D renderTexture { };
	RenderTexture2D radarRenderTexture { };
	LightingContext lightingContext { };
	Rectangle renderRectangle { };
	float renderScale = 1.0f;
	Nc::Vector2i windowSize { };
};