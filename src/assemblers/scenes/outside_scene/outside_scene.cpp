#include "assemblers/scenes/outside_scene/outside_scene.h"
#include "components/objects/receiver_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"


void OutsideScene::Build(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	const entt::entity receiverEntity = registry.create();
	registry.emplace<Component::Receiver>(receiverEntity);
}