#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
struct GameState;
namespace Nc { class ResourceStore; };
struct Settings;


namespace Structure
{

class SettingsMenu final
{
public:
	static void Build(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore,
		Settings& settings,
		Settings& pendingSettings,
		GameState& gameState,
		Nc::Vector2f windowSize
	) noexcept;


	static void Toggle(entt::registry& registry, GameState& gameState) noexcept;
	static void Open(entt::registry& registry, GameState& gameState) noexcept;
	static void Close(entt::registry& registry, GameState& gameState) noexcept;
	static void Close(
		entt::registry& registry,
		GameState& gameState,
		Settings& settings,
		Settings& pendingSettings
	) noexcept;

private:
	static void Toggle(entt::registry& registry, GameState& gameState, ToggleState state) noexcept;

};

}