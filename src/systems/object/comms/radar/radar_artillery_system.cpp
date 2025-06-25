#include "components/core/transform_component.h"
#include "components/objects/comms/radar_tags.h"
#include "components/objects/outside/artillery_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/radar_artillery_system.h"
#include "utility/vector2.h"


void RadarArtillerySystem::Update(
	entt::registry& registry, float deltaTime
)
{
	auto view = registry.view<Tag::RadarArtillery, Component::Transform>();
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