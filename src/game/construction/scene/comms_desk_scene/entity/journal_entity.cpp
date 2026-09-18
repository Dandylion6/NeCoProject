#include "game/construction/scene/comms_desk_scene/entity/journal_entity.hpp"

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/scene/comms_desk_scene/journal_component.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"


entt::entity Entity::Journal::Create(const SceneContext& context) noexcept
{
	constexpr char NOTE_ALBEDO_PATH[] = "assets/environment/objects/note/page_n.png";
    constexpr char NOTE_NORMAL_PATH[] = "assets/environment/objects/note/journal_normal.png";
    constexpr char NOTE_AO_PATH[] = "assets/environment/objects/note/journal_ao.png";
	constexpr auto POSITION = Nc::Vector2f(695.0f, 520.0f);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Journal>(entity);
    context.registry.emplace<Component::Action::Drag>(entity);
	
	const Texture2D& noteAlbedoTexture = context.store.GetTexture(NOTE_ALBEDO_PATH);
    const Texture2D& noteNormalTexture = context.store.GetTexture(NOTE_NORMAL_PATH);
    const Texture2D& noteAoTexture = context.store.GetTexture(NOTE_AO_PATH);

	context.registry.emplace<Component::Sprite>(entity, noteAlbedoTexture, noteNormalTexture, noteAoTexture);

	auto size = Nc::Vector2f(noteAlbedoTexture.width, noteAlbedoTexture.height);
	context.registry.emplace<Component::Transform>(entity, CommsDesk, POSITION, size, size * 0.5f, 1, 3.0f);

	return entity;
}
