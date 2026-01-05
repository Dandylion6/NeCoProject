#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
namespace Nc { class ResourceStore; };
struct SaveContext;


namespace Structure
{

class RestartMenu final
{
public:
	static void Build(
		Game& game,
		entt::registry& registry,
		Nc::ResourceStore& resourceStore,
		GameState& gameState,
		Nc::Vector2f windowSize
	) noexcept;


	static void Open(entt::registry& registry, GameState& gameState) noexcept;
	static void Close(entt::registry& registry, GameState& gameState) noexcept;

private:
	static void Toggle(
		entt::registry& registry, 
		GameState& gameState, 
		bool active
	) noexcept;

};

}
