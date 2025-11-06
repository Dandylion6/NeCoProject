#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/construction/scene/comms_scene/entity/radio_entity.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"


const entt::entity Construct::RadioEntity(entt::registry& registry)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(Nc::RENDER_RESOLUTION) * 0.5f;

	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, CommsRoom, POSITION);
	registry.emplace<Component::SoundEmitter>(entity, Sound { });
	registry.emplace<Component::Radio>(entity);

    return entity;
}
