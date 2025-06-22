#include "assemblers/scenes/comms_scene/radio_entity.h"
#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "components/objects/radio_tag.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"


const entt::entity Construct::RadioEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, CommsRoom);
	registry.emplace<Component::SoundEmitter>(entity, Sound { });
	registry.emplace<Tag::Radio>(entity);

    return entity;
}
