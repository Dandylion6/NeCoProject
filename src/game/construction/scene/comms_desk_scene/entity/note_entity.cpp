#include "game/construction/scene/comms_desk_scene/entity/note_entity.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_desk_scene/note_component.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "core/data/vector2.hpp"
#include <utility>


const entt::entity Construct::NoteEntity(entt::registry& registry, ResourceStore& resourceStore)
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
