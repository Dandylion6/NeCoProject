#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "components/scene/ambient_sound_tag.h"
#include "core/game_state.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/sound_system.h"
#include "systems/scene/ambient_sound_system.h"
#include "utility/interpolation.h"
#include <string>
#include <utility>


void AmbientSoundSystem::Update(
	entt::registry& registry, GameState& gameState, float deltaTime
)
{
	auto view = registry.view<Tag::AmbientSound, Component::Transform, Component::LoopedSoundEmitter>();
	for (auto [entity, transform, emitter] : view.each())
	{
		bool ambientSoundChanged = transform.boundScene == gameState.currentScene;
		if (gameState.movingToScene == NullScene && !ambientSoundChanged)
		{
			emitter.volume = 0.0f;
			TryPlayAmbience(transform, emitter, gameState.currentScene);
			continue;
		}

		constexpr float VOLUME_FADE_SPEED = 8.0f;

		float targetVolume = gameState.movingToScene != NullScene ? 0.0f : 1.0f;
		float volume = Math::SmoothApproach(emitter.volume, targetVolume, deltaTime, VOLUME_FADE_SPEED);
		emitter.volume = volume;
	}
}


void AmbientSoundSystem::TryPlayAmbience(
	Component::Transform& transform, 
	Component::LoopedSoundEmitter& emitter, 
	Scene scene
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
		SoundSystem::StopEmitter(emitter);
		break;
	}

	if (!filePath.empty()) TransitionAmbientAudio(emitter, filePath);
	transform.boundScene = scene;
}


void AmbientSoundSystem::TransitionAmbientAudio(
	Component::LoopedSoundEmitter& emitter, 
	const std::string& filePath
)
{
	SoundSystem::StopEmitter(emitter);

	Music ambience = LoadMusicStream(filePath.c_str());
	emitter.sound = std::move(ambience);

	SoundSystem::PlayEmitter(emitter);

	float soundLength = GetMusicTimeLength(emitter.sound);
	float start = GetRandomValue(0, static_cast<int>(soundLength * 100.0f)) * 0.01f;
	SeekMusicStream(emitter.sound, start);
}
