#include "game/construction/scene/outside_scene/entity/artillery_entity.hpp"
#include "game/construction/scene/outside_scene/outside_scene.hpp"
#include "entt/entity/fwd.hpp"


void OutsideScene::Build(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore)
{
	Construct::ArtilleryEntity(registry);
}