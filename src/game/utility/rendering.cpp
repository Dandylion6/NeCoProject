#include "game/utility/rendering.hpp"

#include "raylib.h"
#include "rlgl.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/render_context.hpp"
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


Nc::Vector2f Renderer::GetWorldPosition(const Nc::RenderContext& context, const Nc::Vector2f position)
{
    auto worldPosition = position;
    worldPosition -= Nc::Vector2f(context.renderRectangle.x, context.renderRectangle.y);
    worldPosition /= context.renderScale;
    return worldPosition;
}


Nc::Vector2f Renderer::GetWorldPosition(const Nc::RenderContext& context, const Vector2 screenPosition)
{
    return GetWorldPosition(context, Nc::Vector2f(screenPosition));
}


void Renderer::DrawSpriteLit(
    const Shader& lightShader,
    const Component::Sprite& sprite,
    const Nc::Vector2f position,
    const Nc::Vector2f origin,
    const float rotation,
    const float scale,
    const Nc::RGBa tint
)
{
    if (sprite.alpha == 0.0f) return;

    rlDrawRenderBatchActive();

    SetShaderValueTexture(lightShader, GetShaderLocation(lightShader, "normals"), sprite.normals);
    SetShaderValueTexture(lightShader, GetShaderLocation(lightShader, "ao"), sprite.ambientOcclusion);

    DrawSprite(sprite, position, origin, rotation, scale, tint);

    rlDrawRenderBatchActive();
}


void Renderer::DrawSprite(
    const Component::Sprite& sprite,
    const Nc::Vector2f position,
    const Nc::Vector2f origin,
    const float rotation,
    const float scale,
    Nc::RGBa tint
)
{
    if (sprite.alpha == 0.0f) return;

    Nc::RGBa::SetAlphaFor(tint, sprite.alpha);

    const auto width = static_cast<float>(sprite.albedo.width);
    const auto height = static_cast<float>(sprite.albedo.height);

    const Nc::Vector2f pixelPosition = Nc::Vector::Round(position);
    const Nc::Vector2f pixelOrigin = Nc::Vector::Round(origin);

    ::DrawTexturePro(
        sprite.albedo,
        Rectangle{0.0f, 0.0f, width, height},
        Rectangle{pixelPosition.x, pixelPosition.y, width * scale, height * scale},
        Vector2(pixelOrigin),
        rotation,
        Color(tint)
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

    const Nc::Vector2f pixelPosition = Nc::Vector::Round(position);
    const Nc::Vector2f pixelOrigin = Nc::Vector::Round(origin);

    ::DrawRectanglePro(
        Rectangle{pixelPosition.x, pixelPosition.y, size.x, size.y},
        Vector2(pixelOrigin),
        rotation,
        Color(fillColor)
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

    const Nc::Vector2f pixelPosition = Nc::Vector::Round(position);
    const Nc::Vector2f pixelOrigin = Nc::Vector::Round(offset);

    ::DrawTextPro(
        font,
        text.text.c_str(),
        Vector2(pixelPosition),
        Vector2(pixelOrigin),
        0.0f,
        fontSize,
        spacing,
        Color(text.color)
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
