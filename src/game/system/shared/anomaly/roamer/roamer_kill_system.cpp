#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/game.hpp"
#include "game/state/game_state.hpp"
#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_kill_system.hpp"

#include "game/contexts/system_context.hpp"


void System::Anomaly::Roamer::Kill::Update(const SystemContext& context) noexcept
{
	const auto view = context.registry.view<const Component::Transform, const Component::Anomaly::Roamer, Component::Health>();
	for (auto [entity, transform, roamer, health] : view.each())
	{
		// Kills all roamers
		if (!GameState::IsNight(context.game.hour))
		{
			health.health = 0;
			continue;
		}

		// Phantom roamers don't kill.
		if (roamer.behaviour == RoamerBehaviour::Phantom)
		{
			// Roamer self-destructs if very close to target.
			const Nc::Vector2f targetPosition = Behaviour::GetTargetPosition(roamer.target);
			if (CanKill(transform.position, targetPosition, 32.0f)) health.health = 0;
			continue;
		}

		switch (roamer.target)
		{
		case RoamerTarget::Bunker:
			UpdateBunkerRoamer(context, transform, health);
			break;
		case RoamerTarget::Artillery:
			// TODO: Add artillery logic.
			UpdateBunkerRoamer(context, transform, health);
			break;
		default: break;
		}
	}
}


void System::Anomaly::Roamer::Kill::UpdateBunkerRoamer(
	const SystemContext& context,
	const Component::Transform& transform,
	Component::Health& health
)
{
	if (!CanKill(transform.position, BUNKER_POSITION))
		return;

	health.health = 0; // Roamer kills itself.

	const SceneContext sceneContext = SceneContext(context.registry, context.store, context.game);
	Game::Death(sceneContext); // Player dies.
}


void System::Anomaly::Roamer::Kill::UpdateArtilleryRoamer(
	entt::registry& registry,
	const Component::Transform& transform,
	Component::Health& health
)
{
	if (!CanKill(transform.position, BUNKER_POSITION))
		return;

	health.health = 0; // Roamer kills itself.
	KillArtillery(registry);
}


bool System::Anomaly::Roamer::Kill::CanKill(
	const Nc::Vector2f roamerPosition, const Nc::Vector2f targetPosition, const float killDistance
)
{
	const float killDistanceSqr = killDistance * killDistance;
	const float sqrDistance = Nc::Vector::SqrDistanceBetween(targetPosition, roamerPosition);
	return sqrDistance <= killDistanceSqr;
}


void System::Anomaly::Roamer::Kill::KillArtillery(entt::registry& registry)
{
	const auto view = registry.view<const Component::Artillery, Component::Health>();
	for (auto [entity, artillery, health] : view.each())
		--health.health;
}