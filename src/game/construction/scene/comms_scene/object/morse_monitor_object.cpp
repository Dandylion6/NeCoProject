#include "game/construction/scene/comms_scene/object/morse_monitor_object.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/tag/scene/comms_scene/morse_monitor_tag.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"


void Construct::MorseMonitorObject(
	entt::registry& registry, ResourceStore& resourceStore
)
{
	Construct::MorseMonitorPointerEntity(registry);

	Construct::MorseMonitorRegionEntity(registry, Component::Morse::MonitorRegion::Dot);
	Construct::MorseMonitorRegionEntity(registry, Component::Morse::MonitorRegion::Dash);

	const entt::entity entity = registry.create();

	constexpr Nc::Hex COLOR = 0xbbc4bbff;

	Nc::Vector2f offset = Nc::Vector2f::Up(MorseMonitor::GAUGE_SIZE.y * 0.5f);
	registry.emplace<Component::Transform>(
		entity, CommsRoom, MorseMonitor::POSITION, MorseMonitor::GAUGE_SIZE, offset
	);
	registry.emplace<Component::Rectangle>(entity, COLOR);

}


const entt::entity Construct::MorseMonitorPointerEntity(
	entt::registry& registry
)
{
	const entt::entity entity = registry.create();

	constexpr Nc::Vector2f POINTER_SIZE = Nc::Vector2f(4.0f, 8.0f);
	constexpr Nc::Hex COLOR = 0xeb4f44ff;

	Nc::Vector2f offset = Nc::Vector2f::Up(4.0f) + POINTER_SIZE * 0.5f;
	registry.emplace<Component::Transform>(
		entity, CommsRoom, MorseMonitor::POSITION, POINTER_SIZE, offset
	);
	registry.emplace<Component::Rectangle>(entity, COLOR);
	registry.emplace<Tag::Morse::Monitor>(entity);

	return entity;
}


const entt::entity Construct::MorseMonitorRegionEntity(
	entt::registry& registry, Component::Morse::MonitorRegion::Region region
)
{
	const entt::entity entity = registry.create();

	constexpr Nc::Vector2f REGION_SIZE = Nc::Vector2f::Up(MorseMonitor::GAUGE_SIZE.y - 2.0f);
	constexpr Nc::Hex COLOR = 0x18232eff;

	registry.emplace<Component::Transform>(
		entity, CommsRoom, MorseMonitor::POSITION, REGION_SIZE, REGION_SIZE * 0.5f
	);
	registry.emplace<Component::Rectangle>(entity, COLOR);
	registry.emplace<Component::Morse::MonitorRegion>(entity, region);

	return entity;
}
