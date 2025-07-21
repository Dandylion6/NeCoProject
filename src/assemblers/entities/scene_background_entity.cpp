#include "assemblers/entities/scene_background_entity.hpp"
#include "components/core/rendering/sprite_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/render_context.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"


const entt::entity Construct::SceneBackgroundEntity(
	Texture2D&& texture, entt::registry& registry, Scene scene
)
{
	const entt::entity entity = registry.create();

	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);
	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.5f;

	registry.emplace<Component::Transform>(entity, scene, position, size, size * 0.5f);
	registry.emplace<Component::Sprite>(entity, texture);

	return entity;
}
