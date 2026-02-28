#include "game/construction/scene/outside_scene/entity/projectile_entity.hpp"

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/scene/outside_scene/projectile_component.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"


entt::entity Entity::Projectile::Create(const SceneContext& context, Nc::Vector2f hitPosition) noexcept
{
	constexpr char HIT_SOUND_PATH[] = "assets/audio/object/artillery_hit.wav";
	constexpr float TRAVEL_TIME = 4.5f;

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(entity, Outside, hitPosition);
	context.registry.emplace<Component::Projectile>(entity, TRAVEL_TIME);

	const Sound& sound = context.store.CreateSoundHandle(HIT_SOUND_PATH);
	context.registry.emplace<Component::ShotAudio>(entity, sound);

	return entity;
}
