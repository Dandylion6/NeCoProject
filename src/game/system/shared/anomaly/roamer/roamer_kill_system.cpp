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


void RoamerKillSystem::Update(
	entt::registry& registry, GameState& gameState, float deltaTime
)
{
	auto view = registry.view<const Component::Transform, const Component::Anomaly::Roamer, Component::Health>();
	for (auto [entity, transform, roamer, health] : view.each())
	{
		// Kills all roamers
		if (!GameState::IsNight(gameState.hour))
		{
			health.health = 0;
			continue;
		}

		// Phantom roamers don't kill.
		if (roamer.behaviour == RoamerBehaviour::Phantom)
		{
			// Roamer self-destructs if very close to target.
			Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);
			if (CanKill(transform.position, targetPosition, 32.0f)) health.health = 0;
			continue;
		}

		switch (roamer.target)
		{
		case RoamerTarget::Bunker:
		{
			UpdateBunkerRoamer(registry, entity, transform, roamer, health, gameState);
			break;
		}
		case RoamerTarget::Artillery:
		{
			UpdateBunkerRoamer(registry, entity, transform, roamer, health, gameState);
			break;
		}
		default:
			break;
		}
	}
}


void RoamerKillSystem::UpdateBunkerRoamer(
	entt::registry& registry, 
	entt::entity entity, 
	const Component::Transform& transform, 
	const Component::Anomaly::Roamer& roamer, 
	Component::Health& health, 
	GameState& gameState
)
{
	if (!CanKill(transform.position, BUNKER_POSITION))
		return;

	health.health = 0; // Roamer kills itself.
	Game::Death(registry, gameState); // Player dies.
}


void RoamerKillSystem::UpdateArtilleryRoamer(
	entt::registry& registry, 
	entt::entity entity, 
	const Component::Transform& transform, 
	const Component::Anomaly::Roamer& roamer,
	Component::Health& health
)
{
	if (!CanKill(transform.position, BUNKER_POSITION))
		return;

	health.health = 0; // Roamer kills itself.
	KillArtillery(registry);
}


bool RoamerKillSystem::CanKill(
	Nc::Vector2f roamerPosition, Nc::Vector2f targetPosition, float killDistance
)
{
	float killDistanceSqr = killDistance * killDistance;
	float sqrDistance = Nc::Vector::SqrDistanceOf(targetPosition, roamerPosition);
	return sqrDistance <= killDistance;
}


void RoamerKillSystem::KillArtillery(entt::registry& registry)
{
	auto view = registry.view<const Component::Artillery, Component::Health>();
	for (auto [entity, artillery, health] : view.each())
		--health.health;
}