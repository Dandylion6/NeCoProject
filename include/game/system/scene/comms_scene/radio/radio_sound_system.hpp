#pragma once
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"


class RadioSoundEmitterSystem
{
public:
	static void Update(
		entt::registry& registry, float deltaTime
	);

	static void Broadcast(
		entt::registry& registry,
		Sound&& sound,
		BroadcastPriority priority
	);

};