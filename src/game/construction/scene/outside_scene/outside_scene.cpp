#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/outside_scene/entity/artillery_entity.hpp"
#include "game/construction/scene/outside_scene/outside_scene.hpp"


void Structure::OutsideScene::Build(
	entt::registry& registry,
	GameState& gameState,
	Nc::ResourceStore& resourceStore
) noexcept
{
	Entity::Artillery::Create(registry);
}