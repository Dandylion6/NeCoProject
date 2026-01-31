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


void System::Anomaly::Roamer::Phaser::Update(const SystemContext& context) noexcept
{
	constexpr Nc::Vector2f IDLE_SECONDS_RANGE = Nc::Vector2f(18.0f, 22.0f);
	// Distance from target that turns into striding movement
	constexpr float STRIDING_DISTANCE = 26.0f;
	constexpr float STRIDING_DISTANCE_SQR = STRIDING_DISTANCE * STRIDING_DISTANCE;

	auto& randomService = context.registry.ctx().get<Nc::Random>();

	const auto view = context.registry.view<Component::Transform, Component::Anomaly::Roamer, Component::Anomaly::Phaser>();
	for (auto [entity, transform, roamer, phaser] : view.each())
	{
		constexpr float MOVE_DISTANCE = 10.6f;

		const Nc::Vector2f targetPosition = Behaviour::GetTargetPosition(roamer.target);
		const Nc::Vector2f toTargetDelta = targetPosition - transform.position;
		const float sqrDistanceToTarget = Nc::Vector::SqrMagnitudeOf(toTargetDelta);

		if (sqrDistanceToTarget <= STRIDING_DISTANCE_SQR)
		{
			ChangeToStrider(context.registry, entity);
			return;
		}

		phaser.secondsUntilNextPoint -= context.deltaTime;
		if (phaser.secondsUntilNextPoint > 0.0f) return;

		phaser.secondsUntilNextPoint = randomService.RangeFloat(IDLE_SECONDS_RANGE.x, IDLE_SECONDS_RANGE.y);

		// Gets the next point to teleport to.
		const int nextPointIndex = (phaser.currentPointIndex + 1u) % phaser.pointCount;
		phaser.currentPointIndex = nextPointIndex;

		Nc::Vector2f& newPosition = phaser.points.at(nextPointIndex);
		const Nc::Vector2f direction = Nc::Vector::Normalized(targetPosition - newPosition);

		// Moves the next point closer to the target.
		newPosition += direction * MOVE_DISTANCE;
		transform.position = newPosition;
	}
}


void System::Anomaly::Roamer::Phaser::ChangeToStrider(entt::registry& registry, const entt::entity entity) noexcept
{
	auto& roamer = registry.get<Component::Anomaly::Roamer>(entity);
	roamer.behaviour = RoamerBehaviour::Strider;

	registry.remove<Component::Anomaly::Phaser>(entity);
	Strider::Spawn(registry, entity);
};
