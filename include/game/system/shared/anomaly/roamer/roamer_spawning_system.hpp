#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
struct AnomalyState;
struct SystemContext;


namespace Nc
{
class Random;
};


class RoamerSpawningSystem final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context, AnomalyState& state) noexcept;
	static entt::entity SpawnRoamer(const SystemContext& context, Nc::Vector2f spawnPoint, AnomalyState& anomalyState) noexcept;
	static Nc::Vector2f GenerateRandomSpawnPoint(Nc::Random& random) noexcept;

private:
	// ------ Functions ------
	static bool ShouldSpawnRoamer(const AnomalyState& anomalyState);

};
