#include "game/construction/shared/entity/environment/ambient_sound_entity.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/core/ambient_sound_tag.hpp"
#include "core/runtime/render_context.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "core/data/vector2.hpp"


const entt::entity Construct::AmbientSoundEntity(
	entt::registry& registry
)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = Nc::RENDER_RESOLUTION * 0.5f;
	registry.emplace<Component::Transform>(entity, NullScene, position);

	registry.emplace<Tag::AmbientSound>(entity);
	registry.emplace<Component::LoopedSoundEmitter>(entity);

	return entity;
}