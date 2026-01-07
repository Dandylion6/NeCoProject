#include "algorithm"
#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/construction/scene/comms_scene/object/radar_object.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/phantom_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/phaser_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_spawning_system.hpp"
#include "raylib.h"
#include <cstdint>


void RoamerSpawningSystem::Update(
	entt::registry& registry, Nc::ResourceStore& resourceStore, GameState& gameState, float deltaTime
)
{
	if (!GameState::IsNight(gameState.hour))
	{
		gameState.anomalyState.nextRoamerSpawnSecondsLeft = 0.0f;
		return;
	}

	uint8_t maxRoamers = AnomalyState::GetMaxRoamers(gameState.anomalyState.intensityLevel);
	if (gameState.anomalyState.totalRoamerCount >= maxRoamers) return;
	
	// @brief Spawn wait interval range in minutes for high attraction.
	constexpr Nc::Vector2f SPAWN_WAIT_HIGH_RANGE = Nc::Vector2f(1.4f, 2.3f);
	// @bried Spawn wait interval range in minutes for low attraction.
	constexpr Nc::Vector2f SPAWN_WAIT_LOW_RANGE = Nc::Vector2f(2.1f, 3.2f);

	float pressureTarget = AnomalyState::GetPressureTarget(gameState.anomalyState.intensityLevel);
	float pressureSurplus = std::clamp<float>(gameState.anomalyState.roamerPressureWeight - pressureTarget, -0.5f, 0.5f);
	float timeScale = 1.0f - pressureSurplus;
	gameState.anomalyState.nextRoamerSpawnSecondsLeft -= deltaTime * timeScale;

	if (!ShouldSpawnRoamer(gameState.anomalyState)) return;

	Nc::Vector2f spawnPoint = RoamerSpawningSystem::GenerateRandomSpawnPoint();
	SpawnRoamer(registry, resourceStore, spawnPoint, gameState.anomalyState);
	
	float attractionFactor = gameState.anomalyState.attractionPercentage * 0.01f;
	Nc::Vector2f range = Nc::Vector::Lerp(SPAWN_WAIT_LOW_RANGE, SPAWN_WAIT_HIGH_RANGE, attractionFactor);
	float nextSpawnSeconds = Nc::Random::Range(range.x, range.y) * 60.0f;
	gameState.anomalyState.nextRoamerSpawnSecondsLeft = nextSpawnSeconds;
}


entt::entity RoamerSpawningSystem::SpawnRoamer(
	entt::registry& registry, Nc::ResourceStore& resourceStore, Nc::Vector2f spawnPoint, AnomalyState& anomalyState
)
{
	constexpr int16_t BASE_HEALTH = 10;

	uint8_t randomBehaviour = Nc::Random::RangeU8(0u, static_cast<uint8_t>(RoamerBehaviour::BEHAVIOUR_COUNT) - 1u);
	uint8_t randomTarget = Nc::Random::RangeU8(0u, static_cast<uint8_t>(RoamerTarget::TARGET_COUNT) - 1u);
	RoamerBehaviour behaviour = static_cast<RoamerBehaviour>(randomBehaviour);
	RoamerTarget target = static_cast<RoamerTarget>(randomTarget);

	entt::entity entity = Construct::RadarBlipEntity(registry, resourceStore, spawnPoint, BASE_HEALTH);
	Component::Anomaly::Roamer& roamer = registry.emplace<Component::Anomaly::Roamer>(entity, behaviour, target);

	switch (behaviour)
	{
	case RoamerBehaviour::Strider:
		StriderBehaviourSystem::Spawn(registry, entity, roamer);
		break;
	case RoamerBehaviour::Phaser:
		PhaserBehaviourSystem::Spawn(registry, entity, roamer);
		break;
	case RoamerBehaviour::Phantom:
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
		float sqrDistanceToArtillery = Nc::Vector::SqrDistanceBetween(position, ARTILLERY_POSITION);
		float sqrDistanceToBunker = Nc::Vector::SqrDistanceBetween(position, BUNKER_POSITION);

		weight = Nc::Math::Remap(SPAWN_WEIGHT_RANGE_SQR, Nc::Vector2f(0.0f, 1.0f), sqrDistanceToArtillery);
		weight *= Nc::Math::Remap(SPAWN_WEIGHT_RANGE_SQR, Nc::Vector2f(0.0f, 1.0f), sqrDistanceToBunker);

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
	return anomalyState.nextRoamerSpawnSecondsLeft <= 0.0f;
}