#pragma once
#include "components/core/sound_emitter_component.h"
#include "entt/entity/fwd.hpp"


class SoundSystem
{
public:
	static void Update(entt::registry& registry, float deltaTime);

	static void PlayEmitter(Component::SoundEmitter& emitter);
	static void StopEmitter(Component::SoundEmitter& emitter);

};