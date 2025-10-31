#pragma once
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;
class ResourceStore;


namespace DoorwayScene
{
	void Build(
		entt::registry& registry, GameState& gameState, ResourceStore& resourceStore, RenderContext& renderContext
	);
}