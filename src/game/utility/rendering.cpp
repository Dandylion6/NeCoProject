#include "game/utility/rendering.hpp"

#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/state/scene.hpp"


bool Renderer::IsRenderableToScreen(const Scene boundScene, const Scene currentScene)
{
	const bool ignoresSceneBounds = boundScene == NullScene;
	const bool isShowingBoundScene = boundScene == currentScene;
	return isShowingBoundScene || ignoresSceneBounds;
}


void Renderer::DrawSprite(
	const Component::Sprite& sprite,
	const Nc::Vector2f position,
	const Nc::Vector2f origin,
	const float rotation,
	const float scale
)
{
	if (sprite.alpha == 0.0f) return;

	auto tintColor = Nc::RGBa(WHITE);
	Nc::RGBa::SetAlphaFor(tintColor, sprite.alpha);

	const auto width = static_cast<float>(sprite.texture.width);
	const auto height = static_cast<float>(sprite.texture.height);

	::DrawTexturePro(
		sprite.texture,
		Rectangle{0.0f, 0.0f, width, height},
		Rectangle{position.x, position.y, width * scale, height * scale},
		Vector2(origin),
		rotation,
		WHITE
	);
}


void Renderer::DrawRectangle(
	const Nc::Vector2f position,
	const Nc::Vector2f size,
	const Nc::RGBa fillColor,
	const Nc::Vector2f origin,
	const float rotation
)
{
	if (fillColor.alpha == 0u) return;

	::DrawRectanglePro(
		Rectangle{position.x, position.y, size.x, size.y},
		Vector2(origin),
		rotation,
		WHITE
	);
}


void Renderer::DrawText(
	const Component::Text& text,
	const Nc::Vector2f position,
	const Nc::Vector2f offset,
	Nc::ResourceStore& resourceStore
)
{
	if (text.color.alpha == 0u) return;

	const Font& font = resourceStore.GetFont(text.style, text.fontSize);
	const auto fontSize = static_cast<float>(text.fontSize);
	const float spacing = text.spacing;

	::DrawTextPro(
		font,
		text.text.c_str(),
		Vector2(position),
		Vector2(offset),
		0.0f,
		fontSize,
		spacing,
		WHITE
	);
}


Nc::Vector2f Renderer::GetTextOffset(const Component::Text& text, Nc::ResourceStore& resourceStore)
{
	const Font& font = resourceStore.GetFont(text.style, text.fontSize);
	const auto fontSize = static_cast<float>(text.fontSize);
	const float spacing = text.spacing;

	const char* cText = text.text.c_str();
	const auto textSize = Nc::Vector2f(MeasureTextEx(font, cText, fontSize, spacing));
	auto offset = Nc::Vector2f::Zero();

	switch (text.alignment)
	{
	case Alignment::Left: offset.y = textSize.y * 0.5f;
		break;
	case Alignment::Center: offset = textSize * 0.5f;
		break;
	case Alignment::BottomLeft: offset.y = textSize.y;
		break;
	default: break;
	}
	return offset;
}
