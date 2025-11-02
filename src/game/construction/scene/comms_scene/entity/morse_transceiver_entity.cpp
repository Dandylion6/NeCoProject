#include "game/construction/scene/comms_scene/entity/morse_transceiver_entity.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/core/audio/sound_emitter_system.hpp"
#include "core/data/vector2.hpp"
#include <utility>



void Construct::MorseTransceiverEntity(entt::registry& registry, ResourceStore& resourceStore)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = Nc::RENDER_RESOLUTION;
	position *= 0.5f;

	registry.emplace<Component::Transform>(entity, CommsRoom, position);
	registry.emplace<Component::Morse::Transceiver>(entity);

	Music morseTone = resourceStore.GetMusic("assets/audio/object/morse_tone.wav");
	Component::LoopedSoundEmitter& emitter = registry.emplace<Component::LoopedSoundEmitter>(entity, std::move(morseTone));
	emitter.volume = 0.0f;
	SoundEmitterSystem::PlayEmitter(emitter);
}