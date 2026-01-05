#include "game/construction/scene/outside_scene/outside_scene.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/outside_scene/entity/artillery_entity.hpp"


void Structure::OutsideScene::Build(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	GameState& gameState
) noexcept
{
	Entity::Artillery::Create(registry);
}