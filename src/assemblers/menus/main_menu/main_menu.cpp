#include "assemblers/menus/main_menu/main_menu.hpp"
#include "assemblers/menus/main_menu/menu_background_entity.hpp"
#include "assemblers/menus/main_menu/menu_button_entities.hpp"
#include "components/core/transform_component.hpp"
#include "components/ui/main_menu_tag.hpp"
#include "core/game.hpp"
#include "core/game_state.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"


void MainMenu::Build(
	Game& game, entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
)
{
	Construct::PlayButtonObject(game, registry, gameState, resourceStore);
	Construct::SettingsButtonObject(registry, gameState, resourceStore);
	Construct::ExitButtonObject(registry, gameState, resourceStore);

	Construct::MainMenuBackgroundEntity(registry, resourceStore);

	Close(registry, gameState);
}


void MainMenu::Open(entt::registry& registry, GameState& gameState)
{
	auto view = registry.view<Tag::MainMenu, Component::UiTransform>();
	for (auto [entity, transform] : view.each())
	{
		transform.isVisible = true;
	}
	gameState.currentScene = NullScene;
	gameState.isPaused = true;
}


void MainMenu::Close(entt::registry& registry, GameState &gameState)
{
	auto view = registry.view<Tag::MainMenu, Component::UiTransform>();
	for (auto [entity, transform] : view.each())
	{
		transform.isVisible = false;
	}
	gameState.isPaused = false;
}