#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/construction/scene/comms_scene/entity/morse_transceiver_entity.hpp"
#include "game/state/scene.hpp"
#include "game/system/core/audio/sound_emitter_system.hpp"
#include "raylib.h"
#include <utility>



void Construct::MorseTransceiverEntity(entt::registry& registry, Nc::ResourceStore& resourceStore)
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(Nc::RENDER_RESOLUTION) * 0.5f;
	
	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, CommsRoom, POSITION);
	registry.emplace<Component::Morse::Transceiver>(entity);

	Music morseTone = resourceStore.GetMusic("assets/audio/object/morse_tone.wav");
	Component::LoopedSoundEmitter& emitter = registry.emplace<Component::LoopedSoundEmitter>(entity, std::move(morseTone));
	emitter.volume = 0.0f;
	SoundEmitterSystem::PlayEmitter(emitter);
}