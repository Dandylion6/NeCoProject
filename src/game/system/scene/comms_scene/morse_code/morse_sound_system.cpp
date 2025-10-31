#include "components/core/sound_emitter_component.hpp"
#include "components/objects/comms/morse_transceiver_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/morse_sound_system.hpp"
#include "utility/interpolation.hpp"


void MorseSoundSystem::Update(
	entt::registry& registry, Scene currentScene, float deltaTime
)
{
	auto view = registry.view<Component::MorseTransceiver, Component::LoopedSoundEmitter>();
	for (auto [entity, transceiver, emitter] : view.each())
	{
		constexpr float FADE_SPEED = 64.0f;
		constexpr float TONE_VOLUME = 1.25f;

		float targetVolume = transceiver.isInputActive ? TONE_VOLUME : 0.0f;
		emitter.volume = Math::SmoothApproach(emitter.volume, targetVolume, deltaTime, FADE_SPEED);
	}
};