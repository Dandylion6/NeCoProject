#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/anomaly/roamers/strider_component.hpp"
#include "components/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/anomaly/roamer/strider_behaviour_system.hpp"
#include "systems/anomaly/roamer_behaviour_system.hpp"
#include "utility/random.hpp"
#include "utility/vector2.hpp"


void StriderBehaviourSystem::Spawn(
	entt::registry& registry, const entt::entity entity, Component::AnomalyRoamer& roamer
)
{
	constexpr Nc::Vector2f MOVE_SPEED_RANGE = Nc::Vector2f(0.16f, 0.21f);
	float moveSpeed = Nc::Random::Range(MOVE_SPEED_RANGE.x, MOVE_SPEED_RANGE.y);
	registry.emplace<Component::Strider>(entity, moveSpeed);
}


void StriderBehaviourSystem::Update(
	entt::registry& registry, 
	const entt::entity entity, 
	Component::Transform& transform, 
	Component::AnomalyRoamer& roamer, 
	float deltaTime
)
{
	Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);
	const Component::Strider& strider = registry.get<Component::Strider>(entity);

	Nc::Vector2f difference = targetPosition - transform.position;
	Nc::Vector2f direction = difference.Normalized();
	transform.position += direction * strider.moveSpeed * deltaTime;
}