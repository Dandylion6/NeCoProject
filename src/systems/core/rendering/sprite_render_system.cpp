#include "components/core/rendering/sprite_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/rendering.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/core/rendering/sprite_render_system.hpp"
#include "utility/vector2.hpp"
#include <cmath>


void SpriteRenderSystem::DrawScreen(
	const entt::entity entity, entt::registry& registry, Nc::Vector2f cameraPosition
)
{
	Component::Transform transform = registry.get<Component::Transform>(entity);
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);

	Nc::Vector2f position = transform.position + cameraPosition;
	Renderer::DrawSprite(sprite, position, transform.offset, transform.rotation);
}


void SpriteRenderSystem::DrawUi(
	const entt::entity entity, entt::registry& registry, Nc::Vector2f windowSize
)
{
	Component::UiTransform transform = registry.get<Component::UiTransform>(entity);
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);

	float scale = (windowSize / transform.size).GetMin();
	scale = std::floorf(scale * 10.0f) * 0.1f;

	Nc::Vector2f anchorPoint = transform.anchor * windowSize;
	Nc::Vector2f position = anchorPoint + transform.offset;
	Nc::Vector2f origin = transform.origin * transform.size * scale;

	Renderer::DrawSprite(sprite, position, origin, transform.rotation, scale);
}
