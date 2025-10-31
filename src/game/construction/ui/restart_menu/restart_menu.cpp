#include "game/construction/ui/restart_menu/entity/restart_background_entity.hpp"
#include "game/construction/ui/restart_menu/object/restart_button_objects.hpp"
#include "game/construction/ui/restart_menu/restart_menu.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/ui/restart_menu_tag.hpp"
#include "game/game.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "core/data/vector2.hpp"


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
		auto view = registry.view<const Tag::RestartMenu, Component::UI::Transform>();
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