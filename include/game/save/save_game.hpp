#pragma once
#include "entt/entity/fwd.hpp"
#include <array>
#include <cstdint>
class Game;
struct GameState;


namespace Save
{
	enum class SaveResult : uint8_t
	{
		Success,
		Failure
	};


	enum class LoadResult : uint8_t
	{
		Success,
		Failure
	};


	SaveResult SaveGame(entt::registry& registry, GameState& gameState);

	LoadResult LoadGame(Game& game, entt::registry& registry, GameState& gameState);
}