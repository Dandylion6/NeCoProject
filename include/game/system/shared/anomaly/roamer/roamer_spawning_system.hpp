#pragma once
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
struct AnomalyState;
struct GameState;
class ResourceStore;


class RoamerSpawningSystem
{
public:
	static void Update(
		entt::registry& registry, ResourceStore& resourceStore, GameState& gameState, float time
	);
	static const entt::entity SpawnRoamer(
		entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f spawnPoint, AnomalyState& anomalyState
	);
	static Nc::Vector2f GenerateRandomSpawnPoint();

private:
	static bool ShouldSpawnRoamer(const AnomalyState& anomalyState);

};