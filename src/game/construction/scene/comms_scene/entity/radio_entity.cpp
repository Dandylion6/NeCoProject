#include "game/construction/scene/comms_scene/entity/radio_entity.hpp"

#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include  "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"


entt::entity Entity::Radio::Create(const SceneContext& context) noexcept
{
	// TODO: Add visuals.
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(Nc::RENDER_RESOLUTION) * 0.5f;

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(entity, CommsRoom, POSITION);
	context.registry.emplace<Component::Radio>(entity);
    context.registry.emplace<Component::AudioEmitter>(entity);
	context.registry.emplace<Component::ShotAudio>(entity);

	return entity;
}
