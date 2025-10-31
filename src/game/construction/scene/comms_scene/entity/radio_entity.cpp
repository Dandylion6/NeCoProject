#include "game/construction/scene/comms_scene/entity/radio_entity.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "core/runtime/render_context.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "core/data/vector2.hpp"


const entt::entity Construct::RadioEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE;
	position *= 0.5f;
	registry.emplace<Component::Transform>(entity, CommsRoom, position);

	registry.emplace<Component::SoundEmitter>(entity, Sound { });
	registry.emplace<Component::Radio>(entity);

    return entity;
}
