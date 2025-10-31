#include "game/construction/scene/outside_scene/entity/artillery_entity.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "core/runtime/render_context.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "core/data/vector2.hpp"
#include <utility>


void Construct::ArtilleryEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.5f;
	registry.emplace<Component::Transform>(entity, Outside, position);

	registry.emplace<Component::Receiver>(entity);
	registry.emplace<Component::Artillery>(entity);
	// Artillery has 3 people, this might be useful in the future
	registry.emplace<Component::Health>(entity, 3);

	Sound sound = LoadSound("assets/audio/object/artillery_fire.wav");
	registry.emplace<Component::SoundEmitter>(entity, std::move(sound));
}