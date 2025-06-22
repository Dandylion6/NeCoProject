#pragma once
#include "components/objects/radio_component.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"


class RadioSoundSystem
{
public:
	static void Update(
		entt::registry& registry
	);

	static void Broadcast(
		entt::registry& registry,
		Sound&& sound,
		BroadcastPriority broadcastPriority
	);

};