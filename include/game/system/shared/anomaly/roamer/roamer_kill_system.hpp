#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
#include "components/anomaly/anomaly_roamer_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/health_component.hpp"
struct GameState;


class RoamerKillSystem
{
public:
	static void Update(
		entt::registry& registry, GameState& gameState, float deltaTime
	);

private:
	static void UpdateBunkerRoamer(
		entt::registry& registry,
		const entt::entity entity,
		const Component::Transform& transform,
		const Component::AnomalyRoamer& roamer,
		Component::Health& health,
		GameState& gameState
	);
	static void UpdateArtilleryRoamer(
		entt::registry& registry,
		const entt::entity entity,
		const Component::Transform& transform,
		const Component::AnomalyRoamer& roamer,
		Component::Health& health
	);
	// @brief Checks if the roamer can kill the target.
	static bool CanKill(
		Nc::Vector2f roamerPosition, Nc::Vector2f targetPosition, float killDistance = 6.0f
	);
	static void KillArtillery(entt::registry& registry);

};