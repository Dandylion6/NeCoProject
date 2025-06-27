#include "assemblers/scenes/comms_scene/radar_object.h"
#include "components/entity/roamer_entity_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/entity/roamer_spawning_system.h"
#include "utility/vector2.h"
#include <cstdint>


void RoamerSpawningSystem::Update(
	entt::registry& registry,
	float deltaTIme
)
{
	//TODO: Add spawn logic
}


void RoamerSpawningSystem::SpawnRoamer(entt::registry& registry)
{
	//TODO: Implement spawn logic
	Nc::Vector2f position = Nc::Vector2f::Zero();
	int16_t health = 10;
	
	const entt::entity entity = Construct::RadarBlipEntity(registry, position, health);
	registry.emplace<Component::RoamerEntity>(entity);
};