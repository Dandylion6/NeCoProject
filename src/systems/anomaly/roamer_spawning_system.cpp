#include "algorithm"
#include "assemblers/scenes/comms_scene/radar_object.hpp"
#include "components/anomaly/anomaly_roamer_component.hpp"
#include "core/state/anomaly_state.hpp"
#include "core/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/anomaly/roamer/phantom_behaviour_system.hpp"
#include "systems/anomaly/roamer/phaser_behaviour_system.hpp"
#include "systems/anomaly/roamer/strider_behaviour_system.hpp"
#include "systems/anomaly/roamer_spawning_system.hpp"
#include "utility/interpolation.hpp"
#include "utility/random.hpp"
#include "utility/vector2.hpp"
#include <cstdint>


void RoamerSpawningSystem::Update(
	entt::registry& registry, ResourceStore& resourceStore, AnomalyState& anomalyState, float deltaTime
)
{
	// todo: Implement roamer limit and more complex spawn logic.
	uint8_t maxRoamers = AnomalyState::GetMaxRoamers(anomalyState.intensityLevel);
	if (anomalyState.totalRoamerCount >= maxRoamers) return;
	
	// @brief Spawn wait interval range in minutes for high attraction.
	constexpr Nc::Vector2f SPAWN_WAIT_HIGH_RANGE = Nc::Vector2f(1.4f, 2.3f);
	// @bried Spawn wait interval range in minutes for low attraction.
	constexpr Nc::Vector2f SPAWN_WAIT_LOW_RANGE = Nc::Vector2f(2.1f, 3.2f);

	float pressureTarget = AnomalyState::GetPressureTarget(anomalyState.intensityLevel);
	float pressureSurplus = std::clamp<float>(anomalyState.roamerPressureWeight - pressureTarget, -0.5f, 0.5f);
	float timeScale = 1.0f - pressureSurplus;
	anomalyState.nextSpawnSecondsLeft -= deltaTime * timeScale;

	if (!ShouldSpawnRoamer(anomalyState)) return;

	Nc::Vector2f spawnPoint = RoamerSpawningSystem::GenerateRandomSpawnPoint();
	SpawnRoamer(registry, resourceStore, spawnPoint, anomalyState);
	
	float attractionFactor = anomalyState.attractionPercentage * 0.01f;
	Nc::Vector2f range = Nc::Vector2f::Lerp(SPAWN_WAIT_LOW_RANGE, SPAWN_WAIT_HIGH_RANGE, attractionFactor);
	float nextSpawnSeconds = Nc::Random::Range(range.x, range.y) * 60.0f;
	anomalyState.nextSpawnSecondsLeft = nextSpawnSeconds;
}


const entt::entity RoamerSpawningSystem::SpawnRoamer(
	entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f spawnPoint, AnomalyState& anomalyState
)
{
	constexpr int16_t BASE_HEALTH = 10;

	uint8_t randomBehaviour = Nc::Random::RangeU8(0u, static_cast<uint8_t>(Component::AnomalyRoamer::BEHAVIOUR_COUNT) - 1u);
	uint8_t randomTarget = Nc::Random::RangeU8(0u, static_cast<uint8_t>(Component::AnomalyRoamer::Target::TARGET_COUNT) - 1u);
	Component::AnomalyRoamer::Behaviour behaviour = static_cast<Component::AnomalyRoamer::Behaviour>(randomBehaviour);
	Component::AnomalyRoamer::Target target = static_cast<Component::AnomalyRoamer::Target>(randomTarget);

	const entt::entity entity = Construct::RadarBlipEntity(registry, resourceStore, spawnPoint, BASE_HEALTH);
	Component::AnomalyRoamer& roamer = registry.emplace<Component::AnomalyRoamer>(entity, behaviour, target);

	switch (behaviour)
	{
	case Component::AnomalyRoamer::Strider:
		StriderBehaviourSystem::Spawn(registry, entity, roamer);
		break;
	case Component::AnomalyRoamer::Phaser:
		PhaserBehaviourSystem::Spawn(registry, entity, roamer);
		break;
	case Component::AnomalyRoamer::Phantom:
		PhantomBehaviourSystem::Spawn(registry, anomalyState, entity, roamer);
		break;
	default:
		break;
	}

	return entity;
}


Nc::Vector2f RoamerSpawningSystem::GenerateRandomSpawnPoint()
{
	Nc::Vector2i worldMin = WORLD_BOUNDS.min;
	Nc::Vector2i worldMax = WORLD_BOUNDS.max;

	constexpr int OVERFLOW_RANGE = 4;
	constexpr Nc::Vector2f SPAWN_WEIGHT_RANGE = Nc::Vector2f(64.0f, 92.0f);
	constexpr Nc::Vector2f SPAWN_WEIGHT_RANGE_SQR = SPAWN_WEIGHT_RANGE * SPAWN_WEIGHT_RANGE;
	constexpr uint8_t MAX_SPAWN_SAMPLES = 32u;

	Nc::Vector2f bestPosition = Nc::Vector2f::Zero();
	float bestWeight = -1.0f;

	for (uint8_t i = 0u; i < MAX_SPAWN_SAMPLES; ++i)
	{
		Nc::Vector2f position = Nc::Vector2f::Zero();
		position.x = static_cast<float>(GetRandomValue(worldMin.x - OVERFLOW_RANGE, worldMax.x + OVERFLOW_RANGE));
		position.y = static_cast<float>(GetRandomValue(worldMin.y - OVERFLOW_RANGE, worldMax.y + OVERFLOW_RANGE));

		float weight = 1.0f;
		float distanceToArtillery = (position - ARTILLERY_POSITION).GetSqrDistance();
		float distanceToBunker = (position - BUNKER_POSITION).GetSqrDistance();

		weight = Math::Remap(SPAWN_WEIGHT_RANGE_SQR, Nc::Vector2f(0.0f, 1.0f), distanceToArtillery);
		weight *= Math::Remap(SPAWN_WEIGHT_RANGE_SQR, Nc::Vector2f(0.0f, 1.0f), distanceToBunker);

		if (weight > bestWeight)
		{
			bestWeight = weight;
			bestPosition = position;
		}

		float deterministicValue = Nc::Random::Range(0.0f, 100.0f);
		if (weight >= deterministicValue) return bestPosition;
	}

	return bestPosition;
}


bool RoamerSpawningSystem::ShouldSpawnRoamer(const AnomalyState& anomalyState)
{
	return anomalyState.nextSpawnSecondsLeft <= 0.0f;
}