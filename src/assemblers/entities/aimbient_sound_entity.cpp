#include "assemblers/entities/ambient_sound_entity.h"
#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "components/scene/ambient_sound_tag.h"
#include "core/render_context.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/vector2.h"


const entt::entity Construct::AmbientSoundEntity(
	entt::registry& registry
)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.5f;
	registry.emplace<Component::Transform>(entity, NullScene, position);

	registry.emplace<Tag::AmbientSound>(entity);
	registry.emplace<Component::LoopedSoundEmitter>(entity);

	return entity;
}