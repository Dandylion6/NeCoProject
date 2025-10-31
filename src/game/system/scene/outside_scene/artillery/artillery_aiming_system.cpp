#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/scene/outside_scene/artillery/artillery_aiming_system.hpp"
#include "core/data/interpolation.hpp"
#include "core/data/vector2.hpp"


void ArtilleryAimingSystem::Update(
	entt::registry& registry, float deltaTime
)
{
	auto view = registry.view<Component::Artillery>();
	for (auto [entity, artillery] : view.each())
	{
		if (artillery.isReadyToFire) continue;
		if (artillery.aimStartupDelay > 0.0f)
		{
			artillery.aimStartupDelay -= deltaTime;
			return;
		}

		Nc::Vector2f targetPosition = artillery.targetPosition;
		artillery.aimPosition.x = Math::SmoothApproach(artillery.aimPosition.x, targetPosition.x, deltaTime, 0.4f);
		artillery.aimPosition.y = Math::SmoothApproach(artillery.aimPosition.y, targetPosition.y, deltaTime, 0.4f);

		Nc::Vector2f difference = artillery.aimPosition - artillery.targetPosition;
		if (difference.GetSqrDistance() > 0.1f) continue;

		artillery.aimPosition = artillery.targetPosition;
		artillery.isReadyToFire = true;
	}
}