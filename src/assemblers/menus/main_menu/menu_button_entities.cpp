#include "assemblers/menus/main_menu/main_menu.hpp"
#include "assemblers/menus/main_menu/menu_button_entities.hpp"
#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "assemblers/ui/label_button_object.hpp"
#include "components/core/transform_component.hpp"
#include "components/scene/dont_destroy_on_load_tag.hpp"
#include "components/ui/main_menu_tag.hpp"
#include "core/game.hpp"
#include "core/game_state.hpp"
#include "core/resource_store.hpp"
#include "core/save_game.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/vector2.hpp"
#include <functional>
#include <utility>


void Construct::PlayButtonObject(
	Game& game, entt::registry& registry, SaveContext& saveContext, GameState& gameState, ResourceStore& resourceStore
)
{
	Component::UiTransform transform = Component::UiTransform(
		Nc::Vector2f(0.5f, 0.5f), Nc::Vector2f::Scale(0.5f)
	);
	
	std::function<void()> onClick = [&game, &saveContext, &gameState, &registry]()
	{
		Save::LoadGame(game, registry, saveContext, gameState);
		MainMenu::Close(registry, gameState);
	};
	 
	LabelButton button = Construct::LabelButtonObject<Tag::MainMenu, Tag::DontDestroyOnLoad>(
		std::move(transform), "PLAY", std::move(onClick), registry, resourceStore
	);
}


void Construct::SettingsButtonObject(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	Component::UiTransform transform = Component::UiTransform(
		Nc::Vector2f(0.5f, 0.56f), Nc::Vector2f::Scale(0.5f)
	);
	
	std::function<void()> onClick = [&gameState, &registry]()
	{
		auto view = registry.view<Tag::MainMenu, Component::UiTransform>();
		for (auto [entity, transform] : view.each()) transform.isVisible = false;
		
		SettingsMenu::Open(registry, gameState);
	};
	 
	LabelButton button = Construct::LabelButtonObject<Tag::MainMenu, Tag::DontDestroyOnLoad>(
		std::move(transform), "SETTINGS", std::move(onClick), registry, resourceStore
	);
}


void Construct::ExitButtonObject(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
)
{
	Component::UiTransform transform = Component::UiTransform(
		Nc::Vector2f(0.5f, 0.62f), Nc::Vector2f::Scale(0.5f)
	);
	
	std::function<void()> onClick = [&gameState]()
	{
		gameState.shouldExit = true;
	};
	 
	LabelButton button = Construct::LabelButtonObject<Tag::MainMenu, Tag::DontDestroyOnLoad>(
		std::move(transform), "EXIT", std::move(onClick), registry, resourceStore
	);
}