#pragma once
#include "raylib.h"
#include <utility>


namespace Component
{
/**
 * @brief Stores the texture and alpha value to render.
 * 
 * Usage example:
 * 
 * ```cpp
 * Texture2D texture = resourceStore.GetTexture("/././.png");
 * 
 * registry.emplace<Component::Transform>(entity, ...);
 * registry.emplace<Component::Sprite>(entity, std::move(texture));
 * ```
 */
struct Sprite final
{
	// ------ Members ------

	Texture2D texture { };
	float alpha = 1.0f;


	// ------ Constructors ------

	constexpr Sprite(Texture2D&& texture, float alpha = 1.0f) : 
		texture(std::move(texture)), alpha(alpha) 
	{ };
};

}