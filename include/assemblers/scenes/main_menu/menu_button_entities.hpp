#pragma once
#include "entt/entity/fwd.hpp"
#include <functional>
struct GameState;
class ResourceStore;


namespace Construct
{
	void PlayButtonObject(
		entt::registry& registry,
		GameState& gameState,
		ResourceStore& resourceStore
	);
}