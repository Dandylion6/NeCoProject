#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/anomaly/roamers/strider_component.hpp"
#include "components/core/rendering/sprite_component.hpp"
#include "components/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/anomaly/roamer/phantom_behaviour_system.hpp"
#include "systems/anomaly/roamer_behaviour_system.hpp"
#include "utility/random.hpp"
#include "utility/vector2.hpp"


void PhantomBehaviourSystem::Spawn(
	entt::registry& registry, const entt::entity entity, Component::AnomalyRoamer& roamer
)
{
	constexpr Nc::Vector2f MOVE_SPEED_RANGE = Nc::Vector2f(0.13f, 0.18f);
	float moveSpeed = Nc::Random::Range(MOVE_SPEED_RANGE.x, MOVE_SPEED_RANGE.y);
	registry.emplace<Component::Strider>(entity, moveSpeed);
}


void PhantomBehaviourSystem::Update(
	entt::registry& registry,
	const entt::entity entity,
	Component::Transform& transform,
	Component::AnomalyRoamer& roamer,
	float deltaTime
)
{
	// @brief Speed multiplier when nearly invisible
	constexpr float HIDDEN_SPEED_MULTIPLIER = 13.0f;

	Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);
	const Component::Strider& strider = registry.get<Component::Strider>(entity);

	float speedMultiplier = 1.0f;
	if (sprite.alpha < 0.2f)
		speedMultiplier = HIDDEN_SPEED_MULTIPLIER;

	Nc::Vector2f difference = targetPosition - transform.position;
	Nc::Vector2f direction = difference.Normalized();
	transform.position += direction * strider.moveSpeed * speedMultiplier * deltaTime;
}