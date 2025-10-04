#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/health_component.hpp"
#include "components/objects/outside/artillery_component.hpp"
#include "core/game.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/anomaly/roamer_kill_system.hpp"
#include "utility/vector2.hpp"


void RoamerKillSystem::Update(
	entt::registry& registry, GameState& gameState, float deltaTime
)
{
	auto view = registry.view<const Component::Transform, const Component::AnomalyRoamer, Component::Health>();
	for (auto [entity, transform, roamer, health] : view.each())
	{
		switch (roamer.target)
		{
		case Component::AnomalyRoamer::Target::Bunker:
		{
			if (!CanKill(transform.position, GameState::BUNKER_POSITION))
				break;

			health.health = 0; // Roamer kills itself.
			Game::Death(registry, gameState); // Player dies.
			break;
		}
		case Component::AnomalyRoamer::Target::Artillery:
		{
			if (!CanKill(transform.position, GameState::BUNKER_POSITION))
				break;

			health.health = 0; // Roamer kills itself.
			KillArtillery(registry);
			break;
		}
		default:
			break;
		}
	}
}


bool RoamerKillSystem::CanKill(
	Nc::Vector2f roamerPosition, Nc::Vector2f targetPosition
)
{
	constexpr float KILL_DISTANCE = 6.0f;
	constexpr float KILL_DISTANCE_SQR = KILL_DISTANCE * KILL_DISTANCE;

	float distance = (targetPosition - roamerPosition).GetSqrDistance();
	if (distance > KILL_DISTANCE_SQR) return false;

	return true;
}


void RoamerKillSystem::KillArtillery(entt::registry& registry)
{
	auto view = registry.view<const Component::Artillery, Component::Health>();
	for (auto [entity, artillery, health] : view.each())
		--health.health;
}