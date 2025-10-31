#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/shared/anomaly/roamer/phaser_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/phaser_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_spawning_system.hpp"
#include "core/data/random.hpp"
#include "core/data/vector2.hpp"
#include <cstdint>
#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"


void PhaserBehaviourSystem::Spawn(
	entt::registry& registry, const entt::entity entity, Component::Anomaly::Roamer& roamer
)
{
	Component::Anomaly::Phaser& phaser = registry.emplace<Component::Anomaly::Phaser>(entity);
	for (uint8_t i = 0u; i < phaser.pointCount; ++i)
	{
		phaser.points[i] = RoamerSpawningSystem::GenerateRandomSpawnPoint();
	}
	Component::Transform& transform = registry.get<Component::Transform>(entity);
	transform.position = phaser.points.at(phaser.currentPointIndex);
}


void PhaserBehaviourSystem::Update(
	entt::registry& registry, 
	const entt::entity entity, 
	Component::Transform& transform, 
	Component::Anomaly::Roamer& roamer, 
	float deltaTime
)
{
	constexpr Nc::Vector2f IDLE_SECONDS_RANGE = Nc::Vector2f(18.0f, 22.0f);
	constexpr float MOVE_DISTANCE = 10.6f;
	// @brief Distance from target that turns into striding movement
	constexpr float STRIDING_DISTANCE = 26.0f;
	constexpr float STRIDING_DISTANCE_SQR = STRIDING_DISTANCE * STRIDING_DISTANCE;

	Component::Anomaly::Phaser& phaser = registry.get<Component::Anomaly::Phaser>(entity);
	phaser.secondsUntilNextPoint -= deltaTime;
	if (phaser.secondsUntilNextPoint > 0.0f) return;

	phaser.currentPointIndex = (phaser.currentPointIndex + 1u) % phaser.pointCount;
	phaser.secondsUntilNextPoint = Nc::Random::Range(IDLE_SECONDS_RANGE.x, IDLE_SECONDS_RANGE.y);
	
	Nc::Vector2f& nextPoint = phaser.points.at(phaser.currentPointIndex);
	Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);
	Nc::Vector2f difference = targetPosition - nextPoint;

	bool switchToStriding = difference.GetSqrDistance() <= STRIDING_DISTANCE_SQR;
	if (switchToStriding)
	{
		roamer.behaviour = Component::Anomaly::Roamer::Behaviour::Strider;
		registry.remove<Component::Anomaly::Phaser>(entity);
		StriderBehaviourSystem::Spawn(registry, entity, roamer);
		return;
	}

	// Moves the next point closer to the target.
	Nc::Vector2f direction = difference.Normalized();
	nextPoint += direction * MOVE_DISTANCE;
	transform.position = nextPoint;
};