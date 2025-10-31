#pragma once
#include "components/objects/comms/radio_component.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"


class RadioSoundSystem
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