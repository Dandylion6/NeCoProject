#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/system/core/rendering/sprite_render_system.hpp"
#include "game/utility/rendering.hpp"
#include <cmath>


void SpriteRenderSystem::DrawScreen(
	entt::entity entity, entt::registry& registry, Nc::Vector2f cameraPosition
)
{
	Component::Transform transform = registry.get<Component::Transform>(entity);
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);

	Nc::Vector2f position = transform.position + cameraPosition;
	Renderer::DrawSprite(sprite, position, transform.offset, transform.rotation);
}


void SpriteRenderSystem::DrawUi(
	entt::entity entity, entt::registry& registry, Nc::Vector2f windowSize
)
{
	Component::UI::Transform transform = registry.get<Component::UI::Transform>(entity);
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);

	float scale = Nc::Vector::MinOf(windowSize / transform.size);
	scale = std::floorf(scale * 10.0f) * 0.1f;

	Nc::Vector2f anchorPoint = transform.anchor * windowSize;
	Nc::Vector2f position = anchorPoint + transform.offset;
	Nc::Vector2f origin = transform.origin * transform.size * scale;

	Renderer::DrawSprite(sprite, position, origin, transform.rotation, scale);
}
