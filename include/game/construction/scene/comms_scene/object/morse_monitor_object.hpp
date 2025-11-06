#pragma once
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
namespace Nc { class ResourceStore; };


namespace MorseMonitor
{
	constexpr Nc::Vector2f GAUGE_SIZE = Nc::Vector2f(98.0f, 8.0f);
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(280.0f, 440.0f);
}


namespace Construct
{
	void MorseMonitorObject(
		entt::registry& registry, Nc::ResourceStore& resourceStore
	);

	const entt::entity MorseMonitorPointerEntity(entt::registry& registry);

	const entt::entity MorseMonitorRegionEntity(
		entt::registry& registry, 
		Component::Morse::MonitorRegion::Region region
	);
}