#pragma once
#include "entt/entity/fwd.hpp"
#include <string>
struct GameState;


namespace Save
{
	bool SaveGame(entt::registry& registry, GameState& gameState);

	bool LoadGame(entt::registry& registry, GameState& gameState);
}