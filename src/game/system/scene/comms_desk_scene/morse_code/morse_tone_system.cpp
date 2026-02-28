#include "game/system/scene/comms_desk_scene/morse_code/morse_tone_system.hpp"

#include "core/math/interpolation.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include "game/contexts/system_context.hpp"


void System::Morse::Tone::Update(const SystemContext& context)
{
	constexpr float FADE_SPEED = 54.0f;
	constexpr float TONE_VOLUME = 1.1f;

	const entt::entity entity = entt::get_single<Component::Morse::Transceiver>(context.registry);
	const auto& transceiver = context.registry.get<Component::Morse::Transceiver>(entity);
	auto& emitter = context.registry.get<Component::AudioEmitter>(entity);

	const float targetVolume = transceiver.isPushed ? TONE_VOLUME : 0.0f;
	emitter.volume = Nc::Math::SmoothApproach(emitter.volume, targetVolume, context.deltaTime, FADE_SPEED);
};
