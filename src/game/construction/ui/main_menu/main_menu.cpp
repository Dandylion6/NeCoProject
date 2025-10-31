#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/main_menu/entity/menu_background_entity.hpp"
#include "game/construction/ui/main_menu/object/menu_button_objects.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/ui/main_menu_tag.hpp"
#include "game/game.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/scene.hpp"
#include "game/state/game_state.hpp"
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
	auto view = registry.view<Tag::MainMenu, Component::UI::Transform>();
	for (auto [entity, transform] : view.each())
	{
		transform.isVisible = true;
	}
	gameState.currentScene = NullScene;
	gameState.isPaused = true;
}


void MainMenu::Close(entt::registry& registry, GameState &gameState)
{
	auto view = registry.view<Tag::MainMenu, Component::UI::Transform>();
	for (auto [entity, transform] : view.each())
	{
		transform.isVisible = false;
	}
	gameState.isPaused = false;
}