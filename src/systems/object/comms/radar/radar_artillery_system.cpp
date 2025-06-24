#include "components/core/transform_component.h"
#include "components/objects/comms/radar_tags.h"
#include "components/objects/outside/artillery_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/radar_artillery_system.h"
#include "utility/interpolation.h"


void RadarArtillerySystem::Update(
	entt::registry& registry, float deltaTime
)
{
	auto view = registry.view<Tag::RadarArtillery, Component::Transform>();
	for (auto [entity, transform] : view.each())
	{
		Nc::Vector2f aimPosition = GetArtilleryAimPosition(transform.position, registry, deltaTime);

		transform.position.x = Math::SmoothApproach(transform.position.x, aimPosition.x, deltaTime, 0.4f);
		transform.position.y = Math::SmoothApproach(transform.position.y, aimPosition.y, deltaTime, 0.4f);
		transform.rotation += 60.0f * deltaTime;
	}
};


Nc::Vector2f RadarArtillerySystem::GetArtilleryAimPosition(
	Nc::Vector2f currentPosition, entt::registry& registry, float deltaTime
)
{
	/*TODO: Make a Artillery Target Entity which holds actual aim position, not Artillery. 
	The radar will render based on artillery target transform position.*/
	auto view = registry.view<Component::Artillery>();
	for (auto [entity, artillery] : view.each())
	{
		if (artillery.aimingDelay <= 0.0f)
		{
			return artillery.aimPosition;
		}
		artillery.aimingDelay -= deltaTime;
	}
	return currentPosition;
}