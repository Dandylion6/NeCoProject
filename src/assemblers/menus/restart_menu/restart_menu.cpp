#include "assemblers/menus/restart_menu/restart_background_entity.hpp"
#include "assemblers/menus/restart_menu/restart_buttons.hpp"
#include "assemblers/menus/restart_menu/restart_menu.hpp"
#include "components/core/transform_component.hpp"
#include "components/ui/restart_menu_tag.hpp"
#include "core/game.hpp"
#include "core/game_state.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/vector2.hpp"


void RestartMenu::Build(
	Game& game, entt::registry& registry, GameState& gameState, ResourceStore& resourceStore, Nc::Vector2f windowSize
)
{
	Construct::RestartMenuBackgroundEntity(registry, resourceStore, windowSize);
	Construct::RestartButton(game, registry, resourceStore, gameState);
	Construct::RestartToMainButton(registry, resourceStore, gameState);
	RestartMenu::Close(registry, gameState);
}


namespace RestartMenu
{
	static void Toggle(entt::registry& registry, GameState& gameState, bool active)
	{
		auto view = registry.view<const Tag::RestartMenu, Component::UiTransform>();
		for (auto [entity, transform] : view.each())
		{
			transform.isVisible = active;
		}
		gameState.isPaused = active;
	}
}


void RestartMenu::Open(entt::registry& registry, GameState& gameState)
{
	RestartMenu::Toggle(registry, gameState, true);
}


void RestartMenu::Close(entt::registry& registry, GameState& gameState)
{
	RestartMenu::Toggle(registry, gameState, false);
}