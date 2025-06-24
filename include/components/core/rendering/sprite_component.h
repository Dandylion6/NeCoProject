#pragma once
#include "raylib.h"
#include <utility>


namespace Component
{
	struct Sprite
	{
		Texture2D texture;
		float alpha = 1.0f;

		Sprite(Texture2D texture): texture(texture) { };
		Sprite(Texture2D&& texture): texture(std::move(texture)) { };
	};
}