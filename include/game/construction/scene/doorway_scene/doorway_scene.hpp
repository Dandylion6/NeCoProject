#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc
{
	struct RenderContext;
	class ResourceStore;
}


namespace DoorwayScene
{
	void Build(
		entt::registry& registry, 
		GameState& gameState, 
		Nc::ResourceStore& resourceStore, 
		Nc::RenderContext& renderContext
	);
}