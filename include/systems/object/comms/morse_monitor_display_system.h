#pragma once
#include "components/objects/comms/morse_transceiver_component.h"
#include "core/game_state.h"
#include "entt/entity/fwd.hpp"
#include <optional>

class MorseMonitorDisplaySystem
{
public:
	static void Update(
		entt::registry& registry,
		MorseSettings settings,
		float deltaTime
	);

private:
	static void UpdatePointer(
		entt::registry& registry,
		MorseSettings settings,
		float deltaTime
	);

	static const Component::MorseTransceiver* GetTrasceiver(entt::registry& registry);

	static void SetRegions(
		entt::registry& registry,
		MorseSettings settings
	);

};