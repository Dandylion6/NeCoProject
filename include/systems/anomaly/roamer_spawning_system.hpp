#pragma once
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"


class RoamerSpawningSystem
{
public:
	static void Update(
		entt::registry& registry, 
		AnomalyState& anomalyState,
		float time
	);

	static const entt::entity SpawnRoamer(
		entt::registry& registry, Nc::Vector2f spawnPoint, AnomalyState anomalyState
	);

private:

	static bool ShouldSpawnRoamer(
		AnomalyState anomalyState, float time
	);

	static bool GenerateRandomSpawnPoint(Nc::Vector2f& spawnPoint);

};