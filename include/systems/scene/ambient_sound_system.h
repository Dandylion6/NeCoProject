#pragma once
#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include <string>
#include <unordered_map>
struct GameState;
class ResourceStore;


static const std::unordered_map<Scene, std::string> ambienceMap {
	{ CommsRoom, "assets/audio/ambient/comms_ambience.wav" }
};


class AmbientSoundSystem
{
public:
	static void Update(
		entt::registry& registry, 
		GameState& gameState,
		float deltaTime
	);

private:
	static void TryPlayAmbience(
		Component::Transform& transform,
		Component::LoopedSoundEmitter& emitter,
		Scene scene
	);

	static void TransitionAmbientAudio(
		Component::LoopedSoundEmitter& emitter,
		const std::string& filePath
	);

};