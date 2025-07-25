#pragma once
#include "raylib.h"
#include <utility>


namespace Component
{
	struct Sprite
	{
		Texture2D texture;
		float alpha = 1.0f;

		Sprite(Texture2D texture, float alpha = 1.0f): texture(texture), alpha(alpha) { };
		Sprite(Texture2D&& texture, float alpha = 1.0f): texture(std::move(texture)), alpha(alpha) { };
	};
}