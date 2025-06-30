#include "assemblers/scenes/comms_scene/morse_monitor_object.h"
#include "components/core/rendering/rectangle_component.h"
#include "components/core/transform_component.h"
#include "components/objects/comms/morse_monitor.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/color.h"
#include "utility/vector2.h"


void Construct::MorseMonitorObject(
	entt::registry& registry, ResourceStore& resourceStore
)
{
	Construct::MorseMonitorPointerEntity(registry);

	Construct::MorseMonitorRegionEntity(registry, Component::MorseMonitorRegion::Dot);
	Construct::MorseMonitorRegionEntity(registry, Component::MorseMonitorRegion::Dash);

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
	registry.emplace<Tag::MorseMonitorPointer>(entity);

	return entity;
}


const entt::entity Construct::MorseMonitorRegionEntity(
	entt::registry& registry, Component::MorseMonitorRegion::Region region
)
{
	const entt::entity entity = registry.create();

	constexpr Nc::Vector2f REGION_SIZE = Nc::Vector2f::Up(MorseMonitor::GAUGE_SIZE.y - 2.0f);
	constexpr Nc::Hex COLOR = 0x18232eff;

	registry.emplace<Component::Transform>(
		entity, CommsRoom, MorseMonitor::POSITION, REGION_SIZE, REGION_SIZE * 0.5f
	);
	registry.emplace<Component::Rectangle>(entity, COLOR);
	registry.emplace<Component::MorseMonitorRegion>(entity, region);

	return entity;
}
