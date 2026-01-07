#pragma once
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/state/settings.hpp"
#include "entt/entity/fwd.hpp"


class MorseMonitorDisplaySystem
{
public:
	static void Update(
		entt::registry& registry,
		Settings::Morse settings,
		float deltaTime
	);

private:
	static void UpdatePointer(
		entt::registry& registry,
		Settings::Morse settings,
		float deltaTime
	);

	static const Component::Morse::Transceiver* GetTrasceiver(entt::registry& registry);

	static void SetRegions(
		entt::registry& registry,
		Settings::Morse settings
	);

};