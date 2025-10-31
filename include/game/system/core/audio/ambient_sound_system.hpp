#pragma once
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include <string>
struct GameState;
class ResourceStore;


class AmbientSoundEmitterSystem
{
public:
	static void Update(
		entt::registry& registry, GameState& gameState, ResourceStore& resourceStore, float deltaTime
	);

private:
	static void TryPlayAmbience(
		Component::Transform& transform, Component::LoopedSoundEmitter& emitter, Scene scene, ResourceStore& resourceStore
	);
	static void TransitionAmbientAudio(
		Component::LoopedSoundEmitter& emitter, const std::string& filePath, ResourceStore& resourceStore
	);

};