#pragma once
#include "components/core/rendering/sprite_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "scene.hpp"
#include "utility/color.hpp"
#include "utility/vector2.hpp"
class ResourceStore;


namespace Renderer
{
	bool IsRenderableToScreen(Scene boundScene, Scene currentScene);

	void DrawSprite(
		const Component::Sprite& sprite,
		Nc::Vector2f position,
		Nc::Vector2f origin = Nc::Vector2f::Zero(),
		float rotation = 0.0f,
		float scale = 1.0f
	);

	void DrawRectangle(
		Nc::Vector2f position,
		Nc::Vector2f size,
		Nc::RGBa fillColor,
		Nc::Vector2f origin = Nc::Vector2f::Zero(),
		float rotation = 0.0f
	);
	
	void DrawText(
		const Component::Text& text,
		Nc::Vector2f position,
		ResourceStore& resourceStore
	);

}