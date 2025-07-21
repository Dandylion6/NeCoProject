#pragma once
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"


class RoamerSpawningSystem
{
public:
	static void Update(
		entt::registry& registry, 
		AnomalyState& anomalyState,
		float time
	);

	static const entt::entity SpawnRoamer(
		entt::registry& registry,
		AnomalyState anomalyState
	);

private:
	static bool ShouldSpawnRoamer(
		AnomalyState anomalyState, float time
	);

};