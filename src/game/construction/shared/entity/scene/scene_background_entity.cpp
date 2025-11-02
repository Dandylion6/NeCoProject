#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "core/runtime/render_context.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "core/data/vector2.hpp"
#include <utility>


const entt::entity Construct::SceneBackgroundEntity(
	Texture2D&& texture, entt::registry& registry, Scene scene
)
{
	const entt::entity entity = registry.create();

	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);
	Nc::Vector2f position = Nc::RENDER_RESOLUTION * 0.5f;

	registry.emplace<Component::Transform>(entity, scene, position, size, size * 0.5f);
	registry.emplace<Component::Sprite>(entity, std::move(texture));

	return entity;
}
