#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/shared/entity/environment/ambient_sound_entity.hpp"
#include "game/state/scene.hpp"
#include "game/tag/core/ambient_sound_tag.hpp"


const entt::entity Entity::AmbientSound::Create(entt::registry& registry) noexcept
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(Nc::RENDER_RESOLUTION) * 0.5f;

	const entt::entity entity = registry.create();

	registry.emplace<Tag::AmbientSound>(entity);

	registry.emplace<Component::Transform>(entity, NullScene, POSITION);
	registry.emplace<Component::LoopedSoundEmitter>(entity);

	return entity;
}