#include "components/core/sound_emitter_component.h"
#include "components/objects/morse_transceiver_component.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/morse_sound_system.h"
#include "utility/interpolation.h"


void MorseSoundSystem::Update(
	entt::registry& registry, Scene currentScene, float deltaTime
)
{
	auto view = registry.view<Component::MorseTransceiver, Component::LoopedSoundEmitter>();
	for (auto [entity, transceiver, emitter] : view.each())
	{
		constexpr float FADE_SPEED = 56.0f;
		constexpr float TONE_VOLUME = 1.25f;

		float targetVolume = transceiver.isInputActive ? TONE_VOLUME : 0.0f;
		emitter.volume = Math::SmoothApproach(emitter.volume, targetVolume, deltaTime, FADE_SPEED);
	}
};