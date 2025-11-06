#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc { class ResourceStore; };


namespace OutsideScene
{
	void Build(
		entt::registry& registry,
		GameState& gameState,
		Nc::ResourceStore& resourceStore
	);
}