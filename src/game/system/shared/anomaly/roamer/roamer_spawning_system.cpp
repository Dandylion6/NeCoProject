#include "game/system/shared/anomaly/roamer/roamer_spawning_system.hpp"

#include <cstdint>

#include "algorithm"
#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
#include "game/construction/scene/comms_scene/entity/blip_entity.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/phantom_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/phaser_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/behaviour/strider_behaviour_system.hpp"


void System::Anomaly::Roamer::Spawning::Update(const SystemContext& context, AnomalyState& state) noexcept
{
	// Spawn wait interval range in minutes for high attraction.
	constexpr auto SPAWN_WAIT_HIGH_RANGE = Nc::Vector2f(1.4f, 2.3f);
	// Spawn wait interval range in minutes for low attraction.
	constexpr auto SPAWN_WAIT_LOW_RANGE = Nc::Vector2f(2.1f, 3.2f);

	if (!GameState::IsNight(context.game.hour))
	{
		state.nextRoamerSpawnSecondsLeft = 0.0f;
		return;
	}

	// TODO: Make sure the spawning is fair and respects difficulty. Spawning should avoid being tied to attraction.

	const uint8_t maxRoamers = AnomalyState::GetMaxRoamers(state.intensityLevel);
	if (state.totalRoamerCount >= maxRoamers) return;

	// Manipulate time to ensure a good pressure target.
	const float pressureTarget = AnomalyState::GetPressureTarget(state.intensityLevel);
	const float pressureSurplus = std::clamp(state.roamerPressureWeight - pressureTarget, -0.5f, 0.5f);
	const float timeScale = 1.0f - pressureSurplus;
	state.nextRoamerSpawnSecondsLeft -= context.deltaTime * timeScale;

	if (!ShouldSpawnRoamer(state)) return;

	auto& randomService = context.registry.ctx().get<Nc::Random>();
	const Nc::Vector2f spawnPoint = GenerateRandomSpawnPoint(randomService);
	SpawnRoamer(context, spawnPoint, state);

	const float attractionFactor = state.attractionPercentage * 0.01f;
	const Nc::Vector2f range = Nc::Vector::Lerp(SPAWN_WAIT_LOW_RANGE, SPAWN_WAIT_HIGH_RANGE, attractionFactor);
	const float nextSpawnSeconds = randomService.RangeFloat(range.x, range.y) * 60.0f;
	state.nextRoamerSpawnSecondsLeft = nextSpawnSeconds;
}


entt::entity System::Anomaly::Roamer::Spawning::SpawnRoamer(
	const SystemContext& context,
	const Nc::Vector2f spawnPoint,
	const AnomalyState& anomalyState
) noexcept
{
	constexpr int16_t BASE_HEALTH = 10;

	auto& randomService = context.registry.ctx().get<Nc::Random>();

	uint8_t randomTarget = randomService.RangeU8(0u, static_cast<uint8_t>(RoamerTarget::TARGET_COUNT) - 1u);
	uint8_t randomBehaviour = randomService.RangeU8(0u, static_cast<uint8_t>(RoamerBehaviour::BEHAVIOUR_COUNT) - 1u);
	auto target = static_cast<RoamerTarget>(randomTarget);
	auto behaviour = static_cast<RoamerBehaviour>(randomBehaviour);

	const auto sceneContext = SceneContext(context.registry, context.store, context.game);
	const entt::entity entity = Entity::Blip::Create(sceneContext, spawnPoint, BASE_HEALTH);
	auto& roamer = context.registry.emplace<Component::Anomaly::Roamer>(entity, behaviour, target);

	switch (behaviour)
	{
	case RoamerBehaviour::Strider: Strider::Spawn(context.registry, entity);
		break;
	case RoamerBehaviour::Phaser: Phaser::Spawn(context.registry, entity);
		break;
	case RoamerBehaviour::Phantom: Phantom::Spawn(context.registry, anomalyState, entity, roamer);
		break;
	default: break;
	}

	return entity;
}


Nc::Vector2f System::Anomaly::Roamer::Spawning::GenerateRandomSpawnPoint(Nc::Random& random) noexcept
{
	constexpr auto WORLD_MIN = WORLD_BOUNDS.min;
	constexpr auto WORLD_MAX = WORLD_BOUNDS.max;

	constexpr auto SPAWN_WEIGHT_RANGE = Nc::Vector2f(64.0f, 92.0f);
	constexpr Nc::Vector2f SPAWN_WEIGHT_RANGE_SQR = Nc::Vector::Modulate(SPAWN_WEIGHT_RANGE, SPAWN_WEIGHT_RANGE);
	constexpr uint8_t MAX_SPAWN_SAMPLES = 32u;

	auto bestPosition = Nc::Vector2f::Zero();
	float bestWeight = -1.0f;

	for (uint8_t i = 0u; i < MAX_SPAWN_SAMPLES; ++i)
	{
		constexpr float OVERFLOW_RANGE = 4.0f;

		Nc::Vector2f position = Nc::Vector2f::Zero();
		position.x = random.RangeFloat(WORLD_MIN.x - OVERFLOW_RANGE, WORLD_MAX.x + OVERFLOW_RANGE);
		position.y = random.RangeFloat(WORLD_MIN.y - OVERFLOW_RANGE, WORLD_MAX.y + OVERFLOW_RANGE);

		const float sqrDistanceToArtillery = Nc::Vector::SqrDistanceBetween(position, ARTILLERY_POSITION);
		const float sqrDistanceToBunker = Nc::Vector::SqrDistanceBetween(position, BUNKER_POSITION);

		float weight = Nc::Math::Remap(SPAWN_WEIGHT_RANGE_SQR, Nc::Vector2f(0.0f, 1.0f), sqrDistanceToArtillery);
		weight *= Nc::Math::Remap(SPAWN_WEIGHT_RANGE_SQR, Nc::Vector2f(0.0f, 1.0f), sqrDistanceToBunker);

		if (weight > bestWeight)
		{
			bestWeight = weight;
			bestPosition = position;
		}

		const float deterministicValue = random.RangeFloat(0.0f, 100.0f);
		if (weight >= deterministicValue) return bestPosition;
	}

	return bestPosition;
}


bool System::Anomaly::Roamer::Spawning::ShouldSpawnRoamer(const AnomalyState& anomalyState)
{
	return anomalyState.nextRoamerSpawnSecondsLeft <= 0.0f;
}
