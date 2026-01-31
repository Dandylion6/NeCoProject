#include "game/construction/scene/comms_scene/object/morse_monitor_object.hpp"

#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/state/scene.hpp"
#include "game/tag/scene/comms_scene/morse_monitor_tag.hpp"


void Object::MorseMonitor::Create(const SceneContext& context) noexcept
{
	// TODO: Add visuals.
	Pointer::Create(context);
	Region::Create(context, Component::Morse::MonitorRegion::Dot);
	Region::Create(context, Component::Morse::MonitorRegion::Dash);
	Gauge::Create(context);
}


entt::entity Object::MorseMonitor::Gauge::Create(const SceneContext& context) noexcept
{
	constexpr auto COLOR = Nc::Hex(0xbbc4bbff);
	constexpr Nc::Vector2f OFFSET = Nc::Vector2f::Up(GAUGE_SIZE.y * 0.5f);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(entity, CommsRoom, POSITION, GAUGE_SIZE, OFFSET);
	context.registry.emplace<Component::Rectangle>(entity, COLOR);

	return entity;
}


entt::entity Object::MorseMonitor::Pointer::Create(const SceneContext& context) noexcept
{
	constexpr Nc::Vector2f POINTER_SIZE = Nc::Vector2f(4.0f, 8.0f);
	constexpr Nc::Vector2f OFFSET = Nc::Vector2f::Up(4.0f) + POINTER_SIZE * 0.5f;
	constexpr auto COLOR = Nc::Hex(0xeb4f44ff);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Tag::Morse::Monitor>(entity);

	context.registry.emplace<Component::Transform>(entity, CommsRoom, POSITION, POINTER_SIZE, OFFSET);
	context.registry.emplace<Component::Rectangle>(entity, COLOR);

	return entity;
}


entt::entity Object::MorseMonitor::Region::Create(
	const SceneContext& context,
	Component::Morse::MonitorRegion::Region region
) noexcept
{
	constexpr Nc::Vector2f REGION_SIZE = Nc::Vector2f::Up(GAUGE_SIZE.y - 2.0f);
	constexpr auto COLOR = Nc::Hex(0x18232eff);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(
		entity,
		CommsRoom,
		POSITION,
		REGION_SIZE,
		REGION_SIZE * 0.5f
	);

	context.registry.emplace<Component::Rectangle>(entity, COLOR);
	context.registry.emplace<Component::Morse::MonitorRegion>(entity, region);

	return entity;
}
