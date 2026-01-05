#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/restart_menu/entity/restart_background_entity.hpp"
#include "game/construction/ui/restart_menu/object/restart_button_objects.hpp"
#include "game/construction/ui/restart_menu/restart_menu.hpp"
#include "game/game.hpp"
#include "game/state/game_state.hpp"
#include "game/tag/ui/restart_menu_tag.hpp"


void Structure::RestartMenu::Build(
	Game& game, 
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	GameState& gameState, 
	Nc::Vector2f windowSize
) noexcept 
{
	Object::RestartMenuBackground::Create(registry, resourceStore, windowSize);
	Object::RestartButton::Create(game, registry, resourceStore, gameState);
	Object::RestartToMainButton::Create(registry, resourceStore, gameState);
	RestartMenu::Close(registry, gameState);
}


void Structure::RestartMenu::Open(
	entt::registry& registry, GameState& gameState
) noexcept
{
	RestartMenu::Toggle(registry, gameState, true);
}


void Structure::RestartMenu::Close(
	entt::registry& registry, GameState& gameState
) noexcept
{
	RestartMenu::Toggle(registry, gameState, false);
}


void Structure::RestartMenu::Toggle(
	entt::registry& registry, GameState& gameState, bool active
) noexcept
{
	gameState.isPaused = active;

	auto view = registry.view<const Tag::RestartMenu, Component::UI::Transform>();
	for (auto [entity, transform] : view.each())
		transform.isVisible = active;
}