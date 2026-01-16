#pragma once
#include "raylib.h"


namespace Component
{
/**
 * @brief Stores the texture and alpha value to render.
 * 
 * Usage example:
 * @code
 * Texture2D texture = resourceStore.GetTexture("/././.png");
 * 
 * registry.emplace<Component::Transform>(entity, ...);
 * registry.emplace<Component::Sprite>(entity, std::move(texture));
 * @endcode
 */
struct Sprite final
{
	// ------ Members ------

	Texture2D texture{ };
	float alpha = 1.0f;


	// ------ Constructors ------

	explicit constexpr Sprite(const Texture2D& texture, const float alpha = 1.0f) noexcept
		: texture(texture), alpha(alpha) { };
};

}
