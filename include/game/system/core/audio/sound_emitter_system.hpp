#pragma once
#include "components/core/sound_emitter_component.hpp"
#include "components/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"


class SoundEmitterSystem
{
public:
	static void Update(entt::registry& registry, float deltaTime);

	static void PlayEmitter(Component::SoundEmitter& emitter);
	static void PlayEmitter(Component::LoopedSoundEmitter& emitter);
	static void StopEmitter(Component::SoundEmitter& emitter);
	static void StopEmitter(Component::LoopedSoundEmitter& emitter);

private:
	static void UpdateEmitter(
		const entt::entity entity, 
		Component::Transform& transform, 
		Component::SoundEmitter& emitter, 
		float deltaTime
	);

	static void UpdateLoopedEmitter(
		const entt::entity entity,
		Component::Transform& transform,
		Component::LoopedSoundEmitter& emitter,
		float deltaTime
	);

	static float GetPan(Nc::Vector2f position);

};