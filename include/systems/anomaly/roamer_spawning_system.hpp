#pragma once
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
class ResourceStore;


class RoamerSpawningSystem
{
public:
	static void Update(
		entt::registry& registry, ResourceStore& resourceStore, AnomalyState& anomalyState, float time
	);
	static const entt::entity SpawnRoamer(
		entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f spawnPoint, AnomalyState& anomalyState
	);
	static Nc::Vector2f GenerateRandomSpawnPoint();

private:
	static bool ShouldSpawnRoamer(const AnomalyState& anomalyState);

};