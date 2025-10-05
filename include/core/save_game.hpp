#pragma once
#include "entt/entity/fwd.hpp"
#include <string>
struct GameState;


namespace Save
{
	void SaveGame(entt::registry& registry, GameState& gameState, const std::string& save = "auto");

	void LoadGame(entt::registry& registry, GameState& gameState, const std::string& save = "auto");
}