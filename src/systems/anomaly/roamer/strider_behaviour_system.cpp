#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/anomaly/roamer/strider_behaviour_system.hpp"
#include "systems/anomaly/roamer_behaviour_system.hpp"
#include "utility/vector2.hpp"


void StriderBehvaiourSystem::Update(
	entt::registry& registry, const entt::entity entity, Component::AnomalyRoamer& roamer, float deltaTime
)
{
	constexpr float SPEED_MODIFIER = 0.14f;

	if (!registry.any_of<Component::Transform>(entity))
		return;

	Component::Transform& transform = registry.get<Component::Transform>(entity);
	Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);

	Nc::Vector2f difference = targetPosition - transform.position;
	Nc::Vector2f direction = difference.Normalized();
	transform.position += direction * roamer.speed * deltaTime * SPEED_MODIFIER;
}