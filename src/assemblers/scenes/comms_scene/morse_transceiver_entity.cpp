#include "assemblers/scenes/comms_scene/morse_transceiver_entity.h"
#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "components/objects/morse_transceiver_component.h"
#include "core/render_context.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/sound_system.h"
#include "utility/vector2.h"
#include <utility>



void Construct::MorseTransceiverEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE;
	position *= 0.5f;

	registry.emplace<Component::Transform>(entity, CommsRoom, position);
	registry.emplace<Component::MorseTransceiver>(entity);

	Music morseTone = LoadMusicStream("assets/audio/object/morse_tone.wav");
	Component::LoopedSoundEmitter& emitter = registry.emplace<Component::LoopedSoundEmitter>(entity, std::move(morseTone));
	emitter.volume = 0.0f;
	SoundSystem::PlayEmitter(emitter);
}