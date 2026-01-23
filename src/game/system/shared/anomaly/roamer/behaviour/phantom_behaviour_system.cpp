#include "game/system/shared/anomaly/roamer/behaviour/phantom_behaviour_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/shared/anomaly/roamer/strider_component.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"


void System::Anomaly::Roamer::Phantom::Spawn(
	entt::registry& registry,
	const AnomalyState& anomalyState,
	const entt::entity entity,
	Component::Anomaly::Roamer& roamer
) noexcept
{
	constexpr Nc::Vector2f MOVE_SPEED_RANGE = Nc::Vector2f(0.13f, 0.17f);

	// Phantom doesn't spawn unless other roamers exist
	if (anomalyState.roamerThreatCount > 0u)
	{
		roamer.behaviour = RoamerBehaviour::Strider;
		Strider::Spawn(registry, entity);
		return;
	}

	auto& randomService = registry.ctx().get<Nc::Random>();

	float moveSpeed = randomService.RangeFloat(MOVE_SPEED_RANGE.x, MOVE_SPEED_RANGE.y);
	registry.emplace<Component::Anomaly::Strider>(entity, moveSpeed);
}


void System::Anomaly::Roamer::Phantom::Update(
	entt::registry& registry,
	const entt::entity entity,
	Component::Transform& transform,
	const Component::Anomaly::Roamer& roamer,
	const float deltaTime
) noexcept
{
	// Speed multiplier when nearly invisible
	constexpr float HIDDEN_SPEED_MULTIPLIER = 13.0f;

	const Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);
	const auto& sprite = registry.get<const Component::Sprite>(entity);
	const auto& strider = registry.get<Component::Anomaly::Strider>(entity);

	float speedMultiplier = 1.0f;
	if (sprite.alpha < 0.2f)
		speedMultiplier = HIDDEN_SPEED_MULTIPLIER;

	const Nc::Vector2f difference = targetPosition - transform.position;
	const Nc::Vector2f direction = Nc::Vector::Normalized(difference);
	transform.position += direction * strider.moveSpeed * speedMultiplier * deltaTime;
}
