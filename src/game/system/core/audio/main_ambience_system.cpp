#include "game/system/core/audio/main_ambience_system.hpp"

#include <string>

#include "raylib.h"
#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"
#include "game/tag/core/ambient_sound_tag.hpp"


void System::Audio::MainAmbience::Update(const SystemContext& context)
{
	constexpr float VOLUME_FADE_SPEED = 8.0f;

	const entt::entity entity = entt::get_single<Tag::AmbientSound>(context.registry);
	auto& transform = context.registry.get<Component::Transform>(entity);
	auto& emitter = context.registry.get<Component::LoopedAudio>(entity);

	const bool ambientSoundChanged = transform.boundScene == context.game.currentScene;
	if (context.game.movingToScene == NullScene && !ambientSoundChanged)
	{
		emitter.volume = 0.0f;
		TryPlayAmbience({ context.store, emitter, context.registry }, transform, context.game.currentScene);
		return;
	}

	const float targetVolume = context.game.movingToScene != NullScene ? 0.0f : 1.0f;
	const float volume = Nc::Math::SmoothApproach(
		emitter.volume,
		targetVolume,
		context.deltaTime,
		VOLUME_FADE_SPEED
	);
	emitter.volume = volume;
}


void System::Audio::MainAmbience::TryPlayAmbience(
	const Context& context,
	Component::Transform& transform,
	const Scene scene
)
{
	if (scene == transform.boundScene) return;

	std::string filePath{ };
	switch (scene)
	{
	case CommsRoom:
	case CommsDesk:
	case Doorway:
		// TODO: Add unique ambience.
		filePath = "assets/audio/ambient/comms_ambience.wav";
		break;
	default:
		SoundEmitterSystem::StopEmitter(context.emitter);
		break;
	}

	if (!filePath.empty()) TransitionAmbientAudio(context, filePath);
	transform.boundScene = scene;
}


void System::Audio::MainAmbience::TransitionAmbientAudio(const Context& context, const std::string& filePath)
{
	auto& randomService = context.registry.ctx().get<Nc::Random>();

	SoundEmitterSystem::StopEmitter(context.emitter);

	const Music& ambience = context.store.GetMusic(filePath);
	context.emitter.sound = ambience;

	SoundEmitterSystem::PlayEmitter(context.emitter);

	const float soundLength = GetMusicTimeLength(context.emitter.sound);
	const float randomStart = randomService.RangeFloat(0.0f, soundLength);
	SeekMusicStream(context.emitter.sound, randomStart);
}
