#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/construction/scene/comms_scene/object/morse_monitor_object.hpp"
#include "game/state/scene.hpp"
#include "game/tag/scene/comms_scene/morse_monitor_tag.hpp"


void Object::MorseMonitor::Create(entt::registry& registry, Nc::ResourceStore& resourceStore) noexcept
{
	MorseMonitor::Pointer::Create(registry);

	MorseMonitor::Region::Create(registry, Component::Morse::MonitorRegion::Dot);
	MorseMonitor::Region::Create(registry, Component::Morse::MonitorRegion::Dash);

	MorseMonitor::Gauge::Create(registry);
}


entt::entity Object::MorseMonitor::Gauge::Create(entt::registry& registry) noexcept
{
	constexpr Nc::Hex COLOR = 0xbbc4bbff;
	constexpr Nc::Vector2f OFFSET = Nc::Vector2f::Up(MorseMonitor::GAUGE_SIZE.y * 0.5f);

	entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(
		entity, 
		CommsRoom, 
		MorseMonitor::POSITION, 
		MorseMonitor::GAUGE_SIZE, 
		OFFSET
	);

	registry.emplace<Component::Rectangle>(entity, COLOR);

	return entity;
}


entt::entity Object::MorseMonitor::Pointer::Create(entt::registry& registry) noexcept
{
	constexpr Nc::Vector2f POINTER_SIZE = Nc::Vector2f(4.0f, 8.0f);
	constexpr Nc::Vector2f OFFSET = Nc::Vector2f::Up(4.0f) + POINTER_SIZE * 0.5f;
	constexpr Nc::Hex COLOR = 0xeb4f44ff;

	entt::entity entity = registry.create();

	registry.emplace<Tag::Morse::Monitor>(entity);

	registry.emplace<Component::Rectangle>(entity, COLOR);
	registry.emplace<Component::Transform>(
		entity, 
		CommsRoom, 
		MorseMonitor::POSITION, 
		POINTER_SIZE, 
		OFFSET
	);

	return entity;
}


entt::entity Object::MorseMonitor::Region::Create(
	entt::registry& registry, 
	Component::Morse::MonitorRegion::Region region
) noexcept
{
	constexpr Nc::Vector2f REGION_SIZE = Nc::Vector2f::Up(MorseMonitor::GAUGE_SIZE.y - 2.0f);
	constexpr Nc::Hex COLOR = 0x18232eff;

	entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(
		entity, 
		CommsRoom, 
		MorseMonitor::POSITION, 
		REGION_SIZE, 
		REGION_SIZE * 0.5f
	);
	
	registry.emplace<Component::Rectangle>(entity, COLOR);
	registry.emplace<Component::Morse::MonitorRegion>(entity, region);

	return entity;
}
