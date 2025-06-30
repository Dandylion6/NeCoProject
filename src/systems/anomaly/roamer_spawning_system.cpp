#include "assemblers/scenes/comms_scene/radar_object.h"
#include "components/anomaly/anomaly_roamer_component.h"
#include "core/game_state.h"
#include "core/render_context.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/anomaly/roamer_spawning_system.h"
#include "utility/vector2.h"
#include <cstdint>


void RoamerSpawningSystem::Update(
	entt::registry& registry, AnomalyState& anomalyState, float time
)
{
	if (ShouldSpawnRoamer(anomalyState, time))
	{
		SpawnRoamer(registry, anomalyState);
		
		anomalyState.spawnWaitMinutes = static_cast<float>(GetRandomValue(150, 400)) * 0.01f;
		anomalyState.lastSpawnTime = time;
	}
}


const entt::entity RoamerSpawningSystem::SpawnRoamer(
	entt::registry& registry, AnomalyState anomalyState
)
{
	Nc::Vector2i radarSize = RenderContext::RADAR_SIZE;
	Nc::Vector2f position = Nc::Vector2f::Zero();
	position.x = static_cast<float>(GetRandomValue(-32, radarSize.x + 32));
	position.y = static_cast<float>(GetRandomValue(-32, radarSize.y + 32));
	int16_t health = 10;
	
	const entt::entity entity = Construct::RadarBlipEntity(registry, position, health);
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