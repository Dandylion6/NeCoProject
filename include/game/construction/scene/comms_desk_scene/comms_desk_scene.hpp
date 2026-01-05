#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc 
{ 
	struct RenderContext;
	class ResourceStore;
};


namespace Structure
{

class DeskScene final
{
public:
	static void Build(
		entt::registry& registry,
		GameState& gameState,
		Nc::RenderContext& renderContext,
		Nc::ResourceStore& resourceStore
	) noexcept;

};

}
