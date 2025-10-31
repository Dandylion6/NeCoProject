#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/utility/rendering.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/core/rendering/sprite_render_system.hpp"
#include "core/data/vector2.hpp"
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
	Component::UI::Transform transform = registry.get<Component::UI::Transform>(entity);
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);

	float scale = (windowSize / transform.size).GetMin();
	scale = std::floorf(scale * 10.0f) * 0.1f;

	Nc::Vector2f anchorPoint = transform.anchor * windowSize;
	Nc::Vector2f position = anchorPoint + transform.offset;
	Nc::Vector2f origin = transform.origin * transform.size * scale;

	Renderer::DrawSprite(sprite, position, origin, transform.rotation, scale);
}
