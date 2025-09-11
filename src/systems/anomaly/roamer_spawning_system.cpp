#include "assemblers/scenes/comms_scene/radar_object.hpp"
#include "components/anomaly/anomaly_roamer_component.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/anomaly/roamer_spawning_system.hpp"
#include "utility/interpolation.hpp"
#include "utility/vector2.hpp"
#include <cstdint>


void RoamerSpawningSystem::Update(
	entt::registry& registry, AnomalyState& anomalyState, float time
)
{
	if (!ShouldSpawnRoamer(anomalyState, time)) return;

	Nc::Vector2f spawnPoint = Nc::Vector2f::Zero();
	bool foundSpawnPoint = RoamerSpawningSystem::GenerateRandomSpawnPoint(spawnPoint);
	if (!foundSpawnPoint)
	{
		anomalyState.lastSpawnTime -= 20.0f; // Add extra time when spawn failed
		return;
	}

	SpawnRoamer(registry, spawnPoint, anomalyState);
		
	anomalyState.spawnWaitMinutes = static_cast<float>(GetRandomValue(25, 35)) * 0.1f;
	anomalyState.lastSpawnTime = time;
}


const entt::entity RoamerSpawningSystem::SpawnRoamer(
	entt::registry& registry, Nc::Vector2f spawnPoint, AnomalyState anomalyState
)
{
	int16_t health = 10;
	
	const entt::entity entity = Construct::RadarBlipEntity(registry, spawnPoint, health);
	registry.emplace<Component::AnomalyRoamer>(entity);

	return entity;
}


bool RoamerSpawningSystem::ShouldSpawnRoamer(
	AnomalyState anomalyState, float time
)
{
	float waitSeconds = anomalyState.spawnWaitMinutes * 60.0f;
	if ((time - anomalyState.lastSpawnTime) < waitSeconds) return false;
	return true;
};


bool RoamerSpawningSystem::GenerateRandomSpawnPoint(Nc::Vector2f& spawnPoint)
{
	Nc::Vector2i worldMin = GameState::WORLD_BOUNDS.min;
	Nc::Vector2i worldMax = GameState::WORLD_BOUNDS.max;

	constexpr int OVERFLOW_RANGE = 4;
	constexpr Nc::Vector2f SPAWN_WEIGHT_RANGE = Nc::Vector2f(64.0f, 92.0f);
	constexpr Nc::Vector2f SPAWN_WEIGHT_RANGE_SQR = SPAWN_WEIGHT_RANGE * SPAWN_WEIGHT_RANGE;
	constexpr uint8_t MAX_SPAWN_SAMPLES = 32u;

	Nc::Vector2f position = Nc::Vector2f::Zero();
	for (uint8_t i = 0u; i < MAX_SPAWN_SAMPLES; ++i)
	{
		position.x = static_cast<float>(GetRandomValue(worldMin.x - OVERFLOW_RANGE, worldMax.x + OVERFLOW_RANGE));
		position.y = static_cast<float>(GetRandomValue(worldMin.y - OVERFLOW_RANGE, worldMax.y + OVERFLOW_RANGE));
		
		float weight = 1.0f;
		float distanceToArtillery = (position - GameState::ARTILLERY_POSITION).GetSqrDistance();
		float distanceToBunker = (position - GameState::BUNKER_POSITION).GetSqrDistance();
		
		weight = Math::Remap(SPAWN_WEIGHT_RANGE_SQR, Nc::Vector2f(0.0f, 1.0f), distanceToArtillery);
		weight *= Math::Remap(SPAWN_WEIGHT_RANGE_SQR, Nc::Vector2f(0.0f, 1.0f), distanceToBunker);
		
		float deterministicValue = static_cast<float>(GetRandomValue(0, 100)) * 0.01f;
		if (weight < deterministicValue) continue;
		
		spawnPoint = position;
		return true;
	}

	return false;
}