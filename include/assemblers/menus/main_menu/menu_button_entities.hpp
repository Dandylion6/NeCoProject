#pragma once
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
class ResourceStore;
struct SaveContext;


namespace Construct
{
	void PlayButtonObject(
		Game& game, entt::registry& registry, SaveContext& saveContext, GameState& gameState, ResourceStore& resourceStore
	);

	void SettingsButtonObject(
		entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
	);

	void ExitButtonObject(
		entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
	);
}