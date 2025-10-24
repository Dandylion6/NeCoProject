#include "assemblers/scenes/desk_scene/note_entity.hpp"
#include "components/core/rendering/sprite_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/note_component.hpp"
#include "core/render_context.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"
#include <utility>


const entt::entity Construct::NoteEntity(
	entt::registry& registry, 
	ResourceStore& resourceStore
)
{
	const entt::entity entity = registry.create();

	registry.emplace<Component::Note>(entity);

	Texture2D texture = resourceStore.GetTexture("assets/environment/objects/note/page_0.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.5f;
	registry.emplace<Component::Transform>(entity, CommsDesk, position, size, size * 0.5f);
	registry.emplace<Component::Sprite>(entity, std::move(texture));

	return entity;
}
