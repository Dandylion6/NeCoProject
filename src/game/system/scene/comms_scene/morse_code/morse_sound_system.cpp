#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/scene/comms_scene/morse_code/morse_sound_system.hpp"
#include "core/math/interpolation.hpp"


void MorseSoundEmitterSystem::Update(
	entt::registry& registry, Scene currentScene, float deltaTime
)
{
	auto view = registry.view<Component::Morse::Transceiver, Component::LoopedSoundEmitter>();
	for (auto [entity, transceiver, emitter] : view.each())
	{
		constexpr float FADE_SPEED = 64.0f;
		constexpr float TONE_VOLUME = 1.25f;

		float targetVolume = transceiver.isInputActive ? TONE_VOLUME : 0.0f;
		emitter.volume = Nc::Math::SmoothApproach(emitter.volume, targetVolume, deltaTime, FADE_SPEED);
	}
};