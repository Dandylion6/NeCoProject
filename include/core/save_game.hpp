#pragma once
#include "entt/entity/fwd.hpp"
#include <string>
class Game;
struct GameState;


namespace Save
{
	bool SaveGame(entt::registry& registry, GameState& gameState);

	bool LoadGame(Game& game, entt::registry& registry, GameState& gameState);
}