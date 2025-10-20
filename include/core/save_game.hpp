#pragma once
#include "corecrt.h"
#include "entt/entity/fwd.hpp"
#include <array>
#include <cstdint>
class Game;
struct GameState;


// @brief Metadata for a specific save slot.
struct SaveMetaData
{
	std::time_t lastSavedTime { };
	// @todo: Add in-game date once implemented.
};


struct SaveContext
{
#ifdef DEBUG_BUILD
	static constexpr uint8_t DEBUG_SAVE_SLOT = 0u;
#endif // DEBUG_BUILD
	static constexpr uint8_t MAX_SAVE_SLOTS = 3u;

	std::array<SaveMetaData, MAX_SAVE_SLOTS> saveMetaData = { };
	// @brief The save identifier, defaults to the first slot.
	uint8_t currentSaveSlot = 1u;
};


namespace Save
{
	bool SaveGame(entt::registry& registry, SaveContext& saveContext, GameState& gameState);

	bool LoadGame(Game& game, entt::registry& registry, SaveContext& saveContext, GameState& gameState);
}