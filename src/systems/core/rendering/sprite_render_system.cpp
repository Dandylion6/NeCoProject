#include "components/core/rendering/sprite_component.h"
#include "components/core/transform_component.h"
#include "core/rendering.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/core/rendering/sprite_render_system.h"
#include "utility/bounds.h"
#include "utility/vector2.h"


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
	entt::registry& registry, Nc::Vector2i windowSize
)
{
	auto view = registry.view<const Component::Sprite, const Component::UiTransform>();
	for (auto [entity, sprite, transform] : view.each())
	{
		Nc::Bounds bounds = Nc::Bounds(transform, windowSize);
		Nc::Vector2i position = bounds.min;
		Renderer::DrawSprite(sprite, position, transform.origin * transform.size, transform.rotation);
	}
}
