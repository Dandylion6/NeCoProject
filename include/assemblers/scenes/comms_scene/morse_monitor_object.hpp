#pragma once
#include "components/objects/comms/morse_monitor.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
class ResourceStore;


namespace MorseMonitor
{
	constexpr Nc::Vector2f GAUGE_SIZE = Nc::Vector2f(98.0f, 8.0f);
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(280.0f, 440.0f);
}


namespace Construct
{
	void MorseMonitorObject(
		entt::registry& registry, ResourceStore& resourceStore
	);

	const entt::entity MorseMonitorPointerEntity(entt::registry& registry);

	const entt::entity MorseMonitorRegionEntity(
		entt::registry& registry, Component::MorseMonitorRegion::Region region
	);
}