#pragma once
#include "raylib.h"
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"


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

	RenderTexture2D renderTexture { };
	RenderTexture2D radarRenderTexture { };
	LightingContext lightingContext { };
	Rectangle renderRectangle { };
	float renderScale = 1.0f;
	Nc::Vector2i windowSize { };
};