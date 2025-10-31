#pragma once
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/shared/stat/health_component.hpp"
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
		const Component::Anomaly::Roamer& roamer,
		Component::Health& health,
		GameState& gameState
	);
	static void UpdateArtilleryRoamer(
		entt::registry& registry,
		const entt::entity entity,
		const Component::Transform& transform,
		const Component::Anomaly::Roamer& roamer,
		Component::Health& health
	);
	// @brief Checks if the roamer can kill the target.
	static bool CanKill(
		Nc::Vector2f roamerPosition, Nc::Vector2f targetPosition, float killDistance = 6.0f
	);
	static void KillArtillery(entt::registry& registry);

};