#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/main_menu/entity/menu_background_entity.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/main_menu/object/menu_button_objects.hpp"
#include "game/game.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/tag/ui/main_menu_tag.hpp"


void Structure::MainMenu::Build(
	Game& game, 
	entt::registry& registry, 
	GameState& gameState, 
	Nc::ResourceStore& resourceStore
) noexcept
{
	Object::PlayButton::Create(game, registry, gameState, resourceStore);
	Object::SettingsButton::Create(registry, gameState, resourceStore);
	Object::ExitButton::Create(registry, gameState, resourceStore);

	Entity::MainMenuBackground::Create(registry, resourceStore);

	Close(registry, gameState);
}


void Structure::MainMenu::Open(
	entt::registry& registry, GameState& gameState
) noexcept
{
	auto view = registry.view<Tag::MainMenu, Component::UI::Transform>();
	for (auto [entity, transform] : view.each())
	{
		transform.isVisible = true;
	}
	gameState.currentScene = NullScene;
	gameState.isPaused = true;
}


void Structure::MainMenu::Close(
	entt::registry& registry, GameState &gameState
) noexcept
{
	auto view = registry.view<Tag::MainMenu, Component::UI::Transform>();
	for (auto [entity, transform] : view.each())
	{
		transform.isVisible = false;
	}
	gameState.isPaused = false;
}