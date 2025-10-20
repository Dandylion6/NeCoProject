#pragma once
#include "entt/entity/fwd.hpp"
#include <cstdint>
class Game;
struct GameState;


namespace Save
{
#ifdef DEBUG_BUILD
	constexpr uint8_t DEBUG_SAVE_SLOT = 0u;
#endif // DEBUG_BUILD
	constexpr uint8_t MAX_SAVE_SLOTS = 3u;

	bool SaveGame(entt::registry& registry, GameState& gameState);

	bool LoadGame(Game& game, entt::registry& registry, GameState& gameState);
}