#include "assemblers/scenes/comms_scene/radio_entity.hpp"
#include "components/core/sound_emitter_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/comms/radio_component.hpp"
#include "core/context/render_context.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"


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
