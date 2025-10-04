#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct GameState;


class RoamerKillSystem
{
public:
	static void Update(
		entt::registry& registry, GameState& gameState, float deltaTime
	);

private:
	// @brief Checks if the roamer can kill the target.
	static bool CanKill(
		Nc::Vector2f roamerPosition, Nc::Vector2f targetPosition
	);

	static void KillArtillery(entt::registry& registry);

};