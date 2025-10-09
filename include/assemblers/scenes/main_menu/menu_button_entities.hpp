#pragma once
#include "entt/entity/fwd.hpp"
#include <functional>
class Game;
struct GameState;
class ResourceStore;


namespace Construct
{
	void PlayButtonObject(
		Game& game, entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
	);

	void SettingsButtonObject(
		entt::registry& registry,
		GameState& gameState,
		ResourceStore& resourceStore
	);

	void ExitButtonObject(
		entt::registry& registry,
		GameState& gameState,
		ResourceStore& resourceStore
	);
}