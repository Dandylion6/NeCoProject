#pragma once
#include "components/core/sound_emitter_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include <string>
struct GameState;
class ResourceStore;


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