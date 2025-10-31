#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/scene/comms_scene/radar/radar_artillery_system.hpp"
#include "core/data/vector2.hpp"


void RadarArtillerySystem::Update(
	entt::registry& registry, float deltaTime
)
{
	auto view = registry.view<const Tag::Radar::Artillery, Component::Transform>();
	for (auto [entity, transform] : view.each())
	{
		transform.position = GetArtilleryAimPosition(registry);
		transform.rotation += 60.0f * deltaTime;
	}
};


Nc::Vector2f RadarArtillerySystem::GetArtilleryAimPosition(entt::registry& registry)
{
	auto view = registry.view<Component::Artillery>();
	for (auto [entity, artillery] : view.each())
	{
		return artillery.aimPosition;
	}
	return Nc::Vector2f::Zero();
}