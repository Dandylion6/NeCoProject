#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
struct RenderContext;
class ResourceStore;


namespace CommsScene
{
	void Build(
		entt::registry& registry, 
		RenderContext& renderContext,
		GameState& gameState, 
		ResourceStore& resourceStore
	);
}