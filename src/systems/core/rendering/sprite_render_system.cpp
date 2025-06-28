#include "components/core/rendering/sprite_component.h"
#include "components/core/transform_component.h"
#include "core/rendering.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/core/rendering/sprite_render_system.h"
#include "utility/vector2.h"
#include <cmath>


void SpriteRenderSystem::DrawScreen(
	entt::registry& registry, Scene currentScene, Nc::Vector2f cameraPosition
)
{
	auto view = registry.view<const Component::Sprite, const Component::Transform>();
	for (auto [entity, sprite, transform] : view.each())
	{
		if (!Renderer::IsRenderableToScreen(transform.boundScene, currentScene)) continue;
		Nc::Vector2f position = transform.position + cameraPosition;
		Renderer::DrawSprite(sprite, position, transform.offset, transform.rotation);
	}
}


void SpriteRenderSystem::DrawUI(
	entt::registry& registry, Nc::Vector2f windowSize
)
{
	auto view = registry.view<const Component::Sprite, const Component::UiTransform>();
	for (auto [entity, sprite, transform] : view.each())
	{
		float scale = (windowSize / transform.size).GetMin();
		scale = std::floorf(scale * 10.0f) * 0.1f;

		Nc::Vector2f anchorPoint = transform.anchor * windowSize;
		Nc::Vector2f position = anchorPoint + transform.offset;
		Nc::Vector2f origin = transform.origin * transform.size * scale;

		Renderer::DrawSprite(sprite, position, origin, transform.rotation, scale);
	}
}
