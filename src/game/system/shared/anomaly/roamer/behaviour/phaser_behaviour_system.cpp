#include "game/system/shared/anomaly/roamer/behaviour/phaser_behaviour_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/shared/anomaly/roamer/phaser_component.hpp"
#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_spawning_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"


void System::Anomaly::Roamer::Phaser::Spawn(entt::registry& registry, const entt::entity entity) noexcept
{
	auto& randomService = registry.ctx().get<Nc::Random>();
	auto& phaser = registry.emplace<Component::Anomaly::Phaser>(entity);

	for (uint8_t i = 0u; i < phaser.pointCount; ++i)
		phaser.points[i] = Spawning::GenerateRandomSpawnPoint(randomService);

	auto& transform = registry.get<Component::Transform>(entity);
	transform.position = phaser.points.at(phaser.currentPointIndex);
}


void System::Anomaly::Roamer::Phaser::Update(
	entt::registry& registry,
	const entt::entity entity,
	Component::Transform& transform, 
	Component::Anomaly::Roamer& roamer,
	const float deltaTime
) noexcept
{
	constexpr Nc::Vector2f IDLE_SECONDS_RANGE = Nc::Vector2f(18.0f, 22.0f);
	constexpr float MOVE_DISTANCE = 10.6f;
	// Distance from target that turns into striding movement
	constexpr float STRIDING_DISTANCE = 26.0f;
	constexpr float STRIDING_DISTANCE_SQR = STRIDING_DISTANCE * STRIDING_DISTANCE;

	auto& randomService = registry.ctx().get<Nc::Random>();
	auto& phaser = registry.get<Component::Anomaly::Phaser>(entity);

	phaser.secondsUntilNextPoint -= deltaTime;
	if (phaser.secondsUntilNextPoint > 0.0f) return;

	phaser.currentPointIndex = (phaser.currentPointIndex + 1u) % phaser.pointCount;
	phaser.secondsUntilNextPoint = randomService.RangeFloat(IDLE_SECONDS_RANGE.x, IDLE_SECONDS_RANGE.y);
	
	Nc::Vector2f& nextPoint = phaser.points.at(phaser.currentPointIndex);
	const Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);

	const Nc::Vector2f difference = targetPosition - nextPoint;
	const float sqrDistance = Nc::Vector::SqrMagnitudeOf(difference);

	if (sqrDistance <= STRIDING_DISTANCE_SQR)
	{
		roamer.behaviour = RoamerBehaviour::Strider;
		registry.remove<Component::Anomaly::Phaser>(entity);
		Strider::Spawn(registry, entity);
		return;
	}

	// Moves the next point closer to the target.
	const Nc::Vector2f direction = Nc::Vector::Normalized(difference);
	nextPoint += direction * MOVE_DISTANCE;

	transform.position = nextPoint;
};