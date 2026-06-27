#include "game/construction/scene/comms_desk_scene/object/morse_monitor_object.hpp"

#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
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

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(entity, CommsDesk, POSITION, GAUGE_SIZE, Nc::Vector2f::Zero(), 1, ROTATION);
	context.registry.emplace<Component::Rectangle>(entity, COLOR);

	return entity;
}


entt::entity Object::MorseMonitor::Pointer::Create(const SceneContext& context) noexcept
{
	constexpr Nc::Vector2f POINTER_SIZE = Nc::Vector2f(8.0f, 2.0f);
	constexpr Nc::Vector2f OFFSET = POINTER_SIZE * 0.5f;
	constexpr auto COLOR = Nc::Hex(0xeb4f44ff);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Tag::Morse::Monitor>(entity);

	context.registry.emplace<Component::Transform>(entity, CommsDesk, POSITION, POINTER_SIZE, OFFSET, 3, ROTATION);
	context.registry.emplace<Component::Rectangle>(entity, COLOR);

	return entity;
}


entt::entity Object::MorseMonitor::Region::Create(
	const SceneContext& context,
	Component::Morse::MonitorRegion::Region region
) noexcept
{
	constexpr Nc::Vector2f REGION_SIZE = Nc::Vector2f::Right(GAUGE_SIZE.x - MARGIN_WIDTH * 2.0f);
    //Nc::Hex(0x18232eff)
	constexpr auto COLOR = Nc::Hex(WHITE);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(
		entity,
		CommsDesk,
		POSITION,
		REGION_SIZE,
		REGION_SIZE * 0.5f,
		2,
		ROTATION
	);

	context.registry.emplace<Component::Rectangle>(entity, COLOR);
	context.registry.emplace<Component::Morse::MonitorRegion>(entity, region);

	return entity;
}
