#include "assemblers/scenes/outside_scene/artillery_entity.hpp"
#include "assemblers/scenes/outside_scene/outside_scene.hpp"
#include "entt/entity/fwd.hpp"


void OutsideScene::Build(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore)
{
	Construct::ArtilleryEntity(registry);
}