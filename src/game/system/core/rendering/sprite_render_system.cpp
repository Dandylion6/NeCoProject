#include "game/system/core/rendering/sprite_render_system.hpp"

#include <cmath>

#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/utility/rendering.hpp"


void SpriteRenderSystem::DrawScreen(
    const Shader& lightShader,
	const entt::entity entity,
	entt::registry& registry,
	const Nc::Vector2f cameraPosition
)
{
	const auto& transform = registry.get<Component::Transform>(entity);
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);

	const Nc::Vector2f position = transform.position + cameraPosition;
    if (!sprite.isSpriteSheet)
    {
        Renderer::DrawSpriteLit(lightShader, sprite, position, transform.offset, transform.rotation, sprite.scale, sprite.tint);
        return;
    }

    const auto width = static_cast<float>(sprite.albedo.width);
    const auto height = static_cast<float>(sprite.albedo.height);

    const float sliceWidth = width / static_cast<float>(sprite.maxFrames);
    const float sliceX =  sliceWidth * static_cast<float>(sprite.frame);

    const auto slice = Rectangle{sliceX, 0.0f, sliceWidth, height};
    Renderer::DrawSpriteLit(lightShader, sprite, slice, position, transform.offset, transform.rotation, sprite.scale, sprite.tint);
}


void SpriteRenderSystem::DrawUi(const entt::entity entity, entt::registry& registry, const Nc::Vector2f windowSize)
{
	const auto& transform = registry.get<Component::UI::Transform>(entity);
	const auto& sprite = registry.get<const Component::Sprite>(entity);

	float scale = Nc::Vector::MinOf(windowSize / transform.size);
	scale = std::floor(scale * 10.0f) * 0.1f;

	const Nc::Vector2f anchorPoint = Nc::Vector::Modulate(transform.anchor, windowSize);
	const Nc::Vector2f position = anchorPoint + transform.offset;
	const Nc::Vector2f origin = Nc::Vector::Modulate(transform.origin, transform.size) * scale;

    const auto width = static_cast<float>(sprite.albedo.width);
    const auto height = static_cast<float>(sprite.albedo.height);

    const auto slice = Rectangle{0.0f, 0.0f, width, height};
	Renderer::DrawSprite(sprite, slice, position, origin, transform.rotation, scale, sprite.tint);
}
