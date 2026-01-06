#include "core/data/vector2.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/shared/anomaly/roamer/strider_component.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"


void StriderBehaviourSystem::Spawn(
	entt::registry& registry, entt::entity entity, Component::Anomaly::Roamer& roamer
)
{
	constexpr Nc::Vector2f MOVE_SPEED_RANGE = Nc::Vector2f(0.16f, 0.21f);
	float moveSpeed = Nc::Random::Range(MOVE_SPEED_RANGE.x, MOVE_SPEED_RANGE.y);
	registry.emplace<Component::Anomaly::Strider>(entity, moveSpeed);
}


void StriderBehaviourSystem::Update(
	entt::registry& registry, 
	entt::entity entity, 
	Component::Transform& transform, 
	Component::Anomaly::Roamer& roamer, 
	float deltaTime
)
{
	Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);
	const Component::Anomaly::Strider& strider = registry.get<Component::Anomaly::Strider>(entity);

	Nc::Vector2f difference = targetPosition - transform.position;
	Nc::Vector2f direction = Nc::Vector::Normalized(difference);
	transform.position += direction * strider.moveSpeed * deltaTime;
}