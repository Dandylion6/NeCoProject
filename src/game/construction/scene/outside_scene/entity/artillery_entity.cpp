#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/construction/scene/outside_scene/entity/artillery_entity.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"
#include <cstdint>
#include <utility>


void Construct::ArtilleryEntity(entt::registry& registry)
{
	constexpr Nc::Vector2f position = Nc::Vector2f(Nc::RENDER_RESOLUTION) * 0.5f;
	constexpr uint16_t SQUAD_COUNT = 3u;

	const entt::entity entity = registry.create();

	registry.emplace<Component::Receiver>(entity);
	registry.emplace<Component::Artillery>(entity);
	registry.emplace<Component::Transform>(entity, Outside, position);
	// Artillery has 3 people, this might be useful in the future
	registry.emplace<Component::Health>(entity, SQUAD_COUNT);

	Sound sound = LoadSound("assets/audio/object/artillery_fire.wav");
	registry.emplace<Component::SoundEmitter>(entity, std::move(sound));
}