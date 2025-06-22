#include "assemblers/scenes/outside_scene/outside_scene.h"
#include "components/objects/receiver_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "assemblers/scenes/outside_scene/artillery_entity.h"


void OutsideScene::Build(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore)
{
	Construct::ArtilleryEntity(registry);
}