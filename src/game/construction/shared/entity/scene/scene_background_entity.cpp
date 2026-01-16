#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"
#include <utility>


entt::entity Entity::SceneBackground::Create(entt::registry& registry, const Texture2D& texture, Scene scene) noexcept
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(Nc::RENDER_RESOLUTION) * 0.5f;

	const entt::entity entity = registry.create();

	auto size = Nc::Vector2f(texture.width, texture.height);
	registry.emplace<Component::Transform>(entity, scene, POSITION, size, size * 0.5f);
	registry.emplace<Component::Sprite>(entity, texture);

	return entity;
}
