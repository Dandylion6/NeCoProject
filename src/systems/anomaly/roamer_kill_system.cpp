#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/health_component.hpp"
#include "components/objects/outside/artillery_component.hpp"
#include "core/game.hpp"
#include "core/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/anomaly/roamer_behaviour_system.hpp"
#include "systems/anomaly/roamer_kill_system.hpp"
#include "utility/vector2.hpp"


void RoamerKillSystem::Update(
	entt::registry& registry, GameState& gameState, float deltaTime
)
{
	auto view = registry.view<const Component::Transform, const Component::AnomalyRoamer, Component::Health>();
	for (auto [entity, transform, roamer, health] : view.each())
	{
		// Kills all roamers
		if (!GameState::IsNight(gameState.hour))
		{
			health.health = 0;
			continue;
		}

		// Phantom roamers don't kill.
		if (roamer.behaviour == Component::AnomalyRoamer::Behaviour::Phantom)
		{
			// Roamer self-destructs if very close to target.
			Nc::Vector2f targetPosition = RoamerBehaviourSystem::GetTargetPosition(roamer.target);
			if (CanKill(transform.position, targetPosition, 32.0f)) health.health = 0;
			continue;
		}

		switch (roamer.target)
		{
		case Component::AnomalyRoamer::Target::Bunker:
		{
			UpdateBunkerRoamer(registry, entity, transform, roamer, health, gameState);
			break;
		}
		case Component::AnomalyRoamer::Target::Artillery:
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
	const entt::entity entity, 
	const Component::Transform& transform, 
	const Component::AnomalyRoamer& roamer, 
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
	const entt::entity entity, 
	const Component::Transform& transform, 
	const Component::AnomalyRoamer& roamer,
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
	float distanceSqr = (targetPosition - roamerPosition).GetSqrDistance();
	return distanceSqr <= killDistance;
}


void RoamerKillSystem::KillArtillery(entt::registry& registry)
{
	auto view = registry.view<const Component::Artillery, Component::Health>();
	for (auto [entity, artillery, health] : view.each())
		--health.health;
}