#pragma once
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
namespace Nc { class ResourceStore; };


namespace Structure
{

class MainMenu final
{
public:
	static void Build(
		Game& game, 
		entt::registry& registry, 
		GameState& gameState, 
		Nc::ResourceStore& resourceStore
	) noexcept;


	static void Open(entt::registry& registry, GameState& gameState) noexcept;
	static void Close(entt::registry& registry, GameState& gameState) noexcept;

};

}