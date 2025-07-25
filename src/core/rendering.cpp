#include "components/core/rendering/sprite_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "core/rendering.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "raylib.h"
#include "utility/color.hpp"
#include "utility/vector2.hpp"


bool Renderer::IsRenderableToScreen(Scene boundScene, Scene currentScene)
{
	bool ignoresSceneBounds = boundScene == NullScene;
	bool isShowingBoundScene = boundScene == currentScene;
	return isShowingBoundScene || ignoresSceneBounds;
}


void Renderer::DrawSprite(
	const Component::Sprite& sprite,
	Nc::Vector2f position,
	Nc::Vector2f origin,
	float rotation,
	float scale
)
{
	bool isTransparent = sprite.alpha == 0.0f;
	if (isTransparent) return;

	Nc::RGBa tintColor = WHITE;
	tintColor.SetAlpha(sprite.alpha);
	
	float width = static_cast<float>(sprite.texture.width);
	float height = static_cast<float>(sprite.texture.height);

	::DrawTexturePro(
		sprite.texture,
		Rectangle { 0.0f, 0.0f, width, height },
		Rectangle { position.x, position.y, width * scale, height * scale },
		origin,
		rotation,
		tintColor
	);
}


void Renderer::DrawRectangle(
	Nc::Vector2f position,
	Nc::Vector2f size,
	Nc::RGBa fillColor,
	Nc::Vector2f origin,
	float rotation
)
{
	bool isTransparent = fillColor.alpha == 0u;
	if (isTransparent) return;

	::DrawRectanglePro(
		Rectangle { position.x, position.y, size.x, size.y },
		origin,
		rotation,
		fillColor
	);
}


void Renderer::DrawText(
	const Component::Text& text, 
	Nc::Vector2f position, 
	Nc::Vector2f offset,
	ResourceStore& resourceStore
)
{
	bool isTransparent = text.color.alpha == 0u;
	if (isTransparent) return;

	Font& font = resourceStore.GetFont(text.style, text.fontSize);
	float fontSize = static_cast<float>(text.fontSize);
	float spacing = static_cast<float>(text.spacing);

	::DrawTextPro(
		font, 
		text.text.c_str(),
		position, 
		offset,
		0.0f, 
		fontSize, 
		spacing,
		text.color
	);
}


Nc::Vector2f Renderer::GetTextOffset(const Component::Text& text,  ResourceStore& resourceStore)
{
	Font& font = resourceStore.GetFont(text.style, text.fontSize);
	float fontSize = static_cast<float>(text.fontSize);
	float spacing = static_cast<float>(text.spacing);

	const char* cText = text.text.c_str();
	Nc::Vector2f textSize = MeasureTextEx(font, cText, fontSize, spacing);
	Nc::Vector2f offset = Nc::Vector2f::Zero();
	switch (text.alignment)
	{
	case Component::Text::Left:
		offset.y = textSize.y * 0.5f;
		break;
	case Component::Text::Center: 
		offset = textSize * 0.5f;
		break;
	default: break;
	}
	return offset;
}
