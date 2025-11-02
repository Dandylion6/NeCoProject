#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/core/ambient_sound_tag.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/scene.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/core/audio/sound_emitter_system.hpp"
#include "game/system/core/audio/ambient_sound_system.hpp"
#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include <string>
#include <utility>


void AmbientSoundEmitterSystem::Update(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore, float deltaTime
)
{
	auto view = registry.view<Tag::AmbientSound, Component::Transform, Component::LoopedSoundEmitter>();
	for (auto [entity, transform, emitter] : view.each())
	{
		bool ambientSoundChanged = transform.boundScene == gameState.currentScene;
		if (gameState.movingToScene == NullScene && !ambientSoundChanged)
		{
			emitter.volume = 0.0f;
			TryPlayAmbience(transform, emitter, gameState.currentScene, resourceStore);
			continue;
		}

		constexpr float VOLUME_FADE_SPEED = 8.0f;

		float targetVolume = gameState.movingToScene != NullScene ? 0.0f : 1.0f;
		float volume = Nc::Math::SmoothApproach(emitter.volume, targetVolume, deltaTime, VOLUME_FADE_SPEED);
		emitter.volume = volume;
	}
}


void AmbientSoundEmitterSystem::TryPlayAmbience(
	Component::Transform& transform, Component::LoopedSoundEmitter& emitter, Scene scene, ResourceStore& resourceStore
)
{
	if (scene == transform.boundScene) return;

	std::string filePath { };
	switch (scene)
	{
	case CommsRoom:
		filePath = "assets/audio/ambient/comms_ambience.wav";
		break;
	case CommsDesk:
		filePath = "assets/audio/ambient/comms_ambience.wav";
		break;
	case Doorway:
		filePath = "assets/audio/ambient/comms_ambience.wav";
		break;
	default:
		SoundEmitterSystem::StopEmitter(emitter);
		break;
	}

	if (!filePath.empty()) TransitionAmbientAudio(emitter, filePath, resourceStore);
	transform.boundScene = scene;
}


void AmbientSoundEmitterSystem::TransitionAmbientAudio(
	Component::LoopedSoundEmitter& emitter, const std::string& filePath, ResourceStore& resourceStore
)
{
	SoundEmitterSystem::StopEmitter(emitter);

	Music ambience = resourceStore.GetMusic(filePath.c_str());
	emitter.sound = std::move(ambience);

	SoundEmitterSystem::PlayEmitter(emitter);

	float soundLength = GetMusicTimeLength(emitter.sound);
	float randomStart = Nc::Random::Range(0.0f, soundLength);
	SeekMusicStream(emitter.sound, randomStart);
}
