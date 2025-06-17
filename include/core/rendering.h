#pragma once
#include "components/core/rendering/sprite_component.h"
#include "components/core/rendering/text_component.h"
#include "scene.h"
#include "utility/color.h"
#include "utility/vector2.h"
class ResourceStore;


namespace Renderer
{
	bool IsRenderableToScreen(Scene boundScene, Scene currentScene);

	void DrawSprite(
		const Component::Sprite& sprite,
		Nc::Vector2f position
	);

	void DrawRectangle(
		Nc::Vector2f position,
		Nc::Vector2f size,
		Nc::RGBa fillColor
	);
	
	void DrawText(
		const Component::Text& text,
		Nc::Vector2f position,
		ResourceStore& resourceStore
	);

}