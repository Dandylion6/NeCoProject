#pragma once
#include "entt/entity/fwd.hpp"
#include <functional>
struct GameState;
class ResourceStore;


namespace Construct
{
	const entt::entity PlayButtonEntity(
		entt::registry& registry,
		GameState& gameState,
		ResourceStore& resourceStore
	);
}