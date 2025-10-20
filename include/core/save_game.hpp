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
	bool isUsed = false;
	// @todo: Add in-game date once implemented.
};


struct SaveContext
{
	static constexpr uint8_t MAX_SAVE_SLOTS = 3u;

	std::array<SaveMetaData, MAX_SAVE_SLOTS> saveMetaData = { };
	// @brief The save identifier, defaults to the first slot.
	uint8_t currentSaveSlot = 1u;
};


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


	SaveResult SaveGame(entt::registry& registry, SaveContext& saveContext, GameState& gameState);

	LoadResult LoadGame(Game& game, entt::registry& registry, SaveContext& saveContext, GameState& gameState);

	SaveResult SaveMetaData(SaveContext& saveContext, GameState& gameState);

	LoadResult LoadMetaData(SaveContext& saveContext, GameState& gameState);
}