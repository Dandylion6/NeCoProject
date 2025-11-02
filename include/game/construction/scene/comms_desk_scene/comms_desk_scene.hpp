#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc { struct RenderContext; };
class ResourceStore;


namespace DeskScene
{
	void Build(
		entt::registry& registry,
		RenderContext& renderContext,
		GameState& gameState,
		ResourceStore& resourceStore
	);
}