#include "components/core/rendering/sprite_component.h"
#include "components/core/rendering/text_component.h"
#include "core/rendering.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "raylib.h"
#include "utility/color.h"
#include "utility/vector2.h"


bool Renderer::IsRenderableToScreen(Scene boundScene, Scene currentScene)
{
	bool ignoresSceneBounds = boundScene == NullScene;
	bool isShowingBoundScene = boundScene == currentScene;
	return isShowingBoundScene || ignoresSceneBounds;
}


void Renderer::DrawSprite(
	const Component::Sprite& sprite,
	Nc::Vector2f position
)
{
	bool isTransparent = sprite.alpha == 0.0f;
	if (isTransparent) return;

	Nc::Vector2i pixelPosition = Nc::Vector2f::Round(position);
	Nc::RGBa tintColor = WHITE;
	tintColor.SetAlpha(sprite.alpha);
	::DrawTexture(sprite.texture, pixelPosition.x, pixelPosition.y, tintColor);
}


void Renderer::DrawRectangle(
	Nc::Vector2f position,
	Nc::Vector2f size,
	Nc::RGBa fillColor
)
{
	bool isTransparent = fillColor.alpha == 0u;
	if (isTransparent) return;

	Nc::Vector2i pixelPosition = Nc::Vector2f::Round(position);
	Nc::Vector2i pixelSize = Nc::Vector2f::Round(size);
	::DrawRectangle(
		pixelPosition.x, 
		pixelPosition.y,
		pixelSize.x,
		pixelSize.y,
		fillColor
	);
}


void Renderer::DrawText(
	const Component::Text& text, 
	Nc::Vector2f position, 
	ResourceStore& resourceStore
)
{
	bool isTransparent = text.color.alpha == 0u;
	if (isTransparent) return;

	Font& font = resourceStore.GetFont(text.style);
	float fontSize = static_cast<float>(text.fontSize);

	const char* cText = text.text.c_str();
	Nc::Vector2f textSize = MeasureTextEx(font, cText, fontSize, 0.0f);
	Nc::Vector2f offset = Nc::Vector2f::Zero();
	switch (text.alignment)
	{
	case Component::Text::Center: 
		offset = textSize * 0.5f;
		break;
	default: break;
	}

	::DrawTextPro(
		font, 
		cText,
		position, 
		offset,
		0.0f, 
		fontSize, 
		0.0f, 
		text.color
	);
}
