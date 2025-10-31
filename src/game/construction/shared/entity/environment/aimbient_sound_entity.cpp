#include "assemblers/entities/ambient_sound_entity.hpp"
#include "components/core/sound_emitter_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/scene/ambient_sound_tag.hpp"
#include "core/context/render_context.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/vector2.hpp"


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