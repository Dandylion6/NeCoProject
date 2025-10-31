#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/main_menu/object/menu_button_objects.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/main_menu_tag.hpp"
#include "game/save/save_game.hpp"
#include "game/game.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "core/data/vector2.hpp"
#include <functional>
#include <utility>


void Construct::PlayButtonObject(
	Game& game, entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
)
{
	Component::UI::Transform transform = Component::UI::Transform(
		Nc::Vector2f(0.5f, 0.5f), Nc::Vector2f::Scale(0.5f)
	);
	
	std::function<void()> onClick = [&game, &gameState, &registry]()
	{
		Save::LoadGame(game, registry, gameState);
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
	Component::UI::Transform transform = Component::UI::Transform(
		Nc::Vector2f(0.5f, 0.56f), Nc::Vector2f::Scale(0.5f)
	);
	
	std::function<void()> onClick = [&gameState, &registry]()
	{
		auto view = registry.view<Tag::MainMenu, Component::UI::Transform>();
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
	Component::UI::Transform transform = Component::UI::Transform(
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