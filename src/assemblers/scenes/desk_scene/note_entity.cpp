#include "assemblers/scenes/desk_scene/note_entity.h"
#include "components/core/rendering/sprite_component.h"
#include "components/core/transform_component.h"
#include "components/objects/note_component.h"
#include "core/render_context.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.h"
#include <utility>


const entt::entity Construct::NoteEntity(
	entt::registry& registry, 
	ResourceStore& resourceStore
)
{
	const entt::entity entity = registry.create();

	registry.emplace<Component::Note>(entity);

	Texture2D& texture = resourceStore.GetTexture("assets/environment/objects/note/page_0.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.5f;
	registry.emplace<Component::Transform>(entity, CommsDesk, position, size, size * 0.5f);
	registry.emplace<Component::Sprite>(entity, texture);

	return entity;
}
