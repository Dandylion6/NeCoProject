#pragma once
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc { class ResourceStore; };


namespace Structure
{

class OutsideScene final
{
public:
	static void Build(entt::registry& registry, Nc::ResourceStore& resourceStore, GameState& gameState) noexcept;

};

}