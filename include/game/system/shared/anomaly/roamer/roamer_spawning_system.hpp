#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
struct AnomalyState;
struct GameState;
namespace Nc { class ResourceStore; };


class RoamerSpawningSystem
{
public:
	static void Update(
		entt::registry& registry, Nc::ResourceStore& resourceStore, GameState& gameState, float time
	);
	static const entt::entity SpawnRoamer(
		entt::registry& registry, Nc::ResourceStore& resourceStore, Nc::Vector2f spawnPoint, AnomalyState& anomalyState
	);
	static Nc::Vector2f GenerateRandomSpawnPoint();

private:
	static bool ShouldSpawnRoamer(const AnomalyState& anomalyState);

};