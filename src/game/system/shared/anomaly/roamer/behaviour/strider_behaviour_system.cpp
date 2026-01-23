#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/shared/anomaly/roamer/strider_component.hpp"
#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"


void System::Anomaly::Roamer::Strider::Spawn(entt::registry& registry, const entt::entity entity) noexcept
{
	constexpr Nc::Vector2f MOVE_SPEED_RANGE = Nc::Vector2f(0.16f, 0.21f);

	auto& randomService = registry.ctx().get<Nc::Random>();

	float moveSpeed = randomService.RangeFloat(MOVE_SPEED_RANGE.x, MOVE_SPEED_RANGE.y);
	registry.emplace<Component::Anomaly::Strider>(entity, moveSpeed);
}


void System::Anomaly::Roamer::Strider::Update(
	entt::registry& registry,
	const entt::entity entity,
	Component::Transform& transform,
	const Component::Anomaly::Roamer& roamer,
	const float deltaTime
) noexcept
{
	const Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);
	const Component::Anomaly::Strider& strider = registry.get<Component::Anomaly::Strider>(entity);

	const Nc::Vector2f difference = targetPosition - transform.position;
	const Nc::Vector2f direction = Nc::Vector::Normalized(difference);
	transform.position += direction * strider.moveSpeed * deltaTime;
}