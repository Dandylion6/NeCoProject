#pragma once
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/state/scene.hpp"
namespace Nc { class ResourceStore; };


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
		Nc::Vector2f offset,
		Nc::ResourceStore& resourceStore
	);

	Nc::Vector2f GetTextOffset(const Component::Text& text, Nc::ResourceStore& resourceStore);
}