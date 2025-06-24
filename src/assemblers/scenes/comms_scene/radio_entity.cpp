#include "assemblers/scenes/comms_scene/radio_entity.h"
#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "components/objects/comms/radio_component.h"
#include "core/render_context.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.h"


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
