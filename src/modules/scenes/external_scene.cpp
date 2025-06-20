#include "components/objects/receiver_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "modules/scenes/external_scene.h"


void ExternalScene::Build(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	const entt::entity receiverEntity = registry.create();
	registry.emplace<Component::Receiver>(receiverEntity);
}