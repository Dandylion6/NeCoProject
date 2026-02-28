#include "game/construction/scene/comms_desk_scene/entity/morse_transceiver_entity.hpp"

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/component/core/interactive/input_component.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"


entt::entity Entity::MorseTransceiver::Create(const SceneContext& context) noexcept
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(Nc::RENDER_RESOLUTION) * 0.5f;
	constexpr  char MORSE_TONE_PATH[] = "assets/audio/object/morse_tone.wav";

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Morse::Transceiver>(entity);
	context.registry.emplace<Component::Transform>(entity, CommsDesk, POSITION);
    context.registry.emplace<Component::Action::Input>(entity, KEY_SPACE);

	const Music& morseTone = context.store.GetMusic(MORSE_TONE_PATH);
	context.registry.emplace<Component::LoopedAudio>(entity, morseTone);

    auto& emitter = context.registry.emplace<Component::AudioEmitter>(entity, 0.0f);
	System::Audio::Emitter::PlayEmitter(emitter);

	return entity;
}
