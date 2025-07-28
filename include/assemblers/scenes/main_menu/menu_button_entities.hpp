#pragma once
#include "entt/entity/fwd.hpp"
#include <functional>
struct GameState;
class ResourceStore;


namespace Construct
{
	void PlayButtonObject(
		entt::registry& registry,
		GameState& gameState,
		ResourceStore& resourceStore
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