#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc 
{ 
	struct RenderContext; 
	class ResourceStore;
};


namespace CommsScene
{
	void Build(
		entt::registry& registry, 
		Nc::RenderContext& renderContext,
		GameState& gameState, 
		Nc::ResourceStore& resourceStore
	);
}