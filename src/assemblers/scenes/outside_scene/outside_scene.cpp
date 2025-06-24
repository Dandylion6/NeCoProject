#include "assemblers/scenes/outside_scene/artillery_entity.h"
#include "assemblers/scenes/outside_scene/outside_scene.h"
#include "entt/entity/fwd.hpp"


void OutsideScene::Build(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore)
{
	Construct::ArtilleryEntity(registry);
}