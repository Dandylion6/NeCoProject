#include "game/construction/scene/comms_desk_scene/entity/note_entity.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_desk_scene/note_component.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"
#include <utility>


entt::entity Entity::Note::Create(entt::registry& registry, Nc::ResourceStore& resourceStore) noexcept
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(Nc::RENDER_RESOLUTION) * 0.5f;

	entt::entity entity = registry.create();

	registry.emplace<Component::Note>(entity);
	
	Texture2D texture = resourceStore.GetTexture("assets/environment/objects/note/page_0.png");
	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);

	registry.emplace<Component::Transform>(entity, CommsDesk, POSITION, size, size * 0.5f);
	registry.emplace<Component::Sprite>(entity, std::move(texture));

	return entity;
}
