#include "components/core/rendering/sprite_component.h"
#include "components/core/transform_component.h"
#include "core/render_context.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/entity/scene_entities.h"
#include "utility/vector2.h"


entt::entity Construct::SceneBackgroundEntity(
	Texture2D&& texture, entt::registry& registry, Scene scene
)
{
	const entt::entity entity = registry.create();

	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);
	Nc::Vector2f offset = size * 0.5f;
	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.5f;

	registry.emplace<Component::Transform>(entity, scene, position, size, offset);
	registry.emplace<Component::Sprite>(entity, texture);
	return entity;
}