#include "game/construction/scene/outside_scene/entity/artillery_entity.hpp"

#include <cstdint>

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"


entt::entity Entity::Artillery::Create(entt::registry& registry) noexcept
{
	constexpr char FIRE_SOUND_PATH[] = "assets/audio/object/artillery_fire.wav";
	constexpr uint16_t SQUAD_COUNT = 3u;

	const entt::entity entity = registry.create();

	registry.emplace<Component::Artillery>(entity);
	registry.emplace<Component::Receiver>(entity);

	registry.emplace<Component::Transform>(entity, Outside, ARTILLERY_POSITION);
	registry.emplace<Component::Health>(entity, SQUAD_COUNT);

	const Sound& sound = LoadSound(FIRE_SOUND_PATH);
	registry.emplace<Component::ShotAudio>(entity, sound);
    registry.emplace<Component::AudioEmitter>(entity, false);

	return entity;
}
