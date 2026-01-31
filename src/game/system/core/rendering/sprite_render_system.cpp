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
	const entt::entity entity,
	entt::registry& registry,
	const Nc::Vector2f cameraPosition
)
{
	const auto& transform = registry.get<Component::Transform>(entity);
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);

	const Nc::Vector2f position = transform.position + cameraPosition;
	Renderer::DrawSprite(sprite, position, transform.offset, transform.rotation);
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

	Renderer::DrawSprite(sprite, position, origin, transform.rotation, scale);
}
