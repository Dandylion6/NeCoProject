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
		GameState& gameState, 
		Nc::ResourceStore& resourceStore, 
		Nc::RenderContext& renderContext
	) noexcept;

};

}