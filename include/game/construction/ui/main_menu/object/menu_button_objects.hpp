#pragma once
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
namespace Nc { class ResourceStore; };


namespace Construct
{
	void PlayButtonObject(
		Game& game, entt::registry& registry, GameState& gameState, Nc::ResourceStore& resourceStore
	);

	void SettingsButtonObject(
		entt::registry& registry, GameState& gameState, Nc::ResourceStore& resourceStore
	);

	void ExitButtonObject(
		entt::registry& registry, GameState& gameState, Nc::ResourceStore& resourceStore
	);
}