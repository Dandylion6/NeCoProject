#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc
{
	struct RenderContext;
	class ResourceStore;
}


namespace Structure
{

class DoorwayScene final
{
public:
	static void Build(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore, 
		Nc::RenderContext& renderContext,
		GameState& gameState
	) noexcept;

};

}