#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct GameState;
class ResourceStore;
struct Settings;


namespace SettingsMenu
{
    void Build(
		Settings& settings,
		Settings& pendingSettings,
		GameState& gameState,
		Nc::Vector2f windowSize,
		entt::registry& registry,
		ResourceStore& resourceStore
	);

	
	void Open(entt::registry& registry, GameState& gameState);

	void Close(entt::registry& registry, GameState& gameState);
	void Close(Settings& settings, Settings& pendingSettings, entt::registry& registry, GameState& gameState);

	void Toggle(entt::registry& registry, GameState& gameState);
};