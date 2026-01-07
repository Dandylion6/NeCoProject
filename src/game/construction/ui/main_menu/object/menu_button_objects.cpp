#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/main_menu/object/menu_button_objects.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "game/game.hpp"
#include "game/save/save_game.hpp"
#include "game/state/game_state.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/main_menu_tag.hpp"
#include <functional>
#include <utility>


void Object::PlayButton::Create(
	Game& game, 
	entt::registry& registry, 
	GameState& gameState, 
	Nc::ResourceStore& resourceStore
) noexcept
{
	constexpr Nc::Vector2f ANCHOR_POINT = Nc::Vector2f(0.5f, 0.5f);
	constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

	std::function<void()> onClick = [&game, &gameState, &registry]()
	{
		game.Load();
		Structure::MainMenu::Close(registry, gameState);
	};
	
	Component::UI::Transform transform = Component::UI::Transform(ANCHOR_POINT, ORIGIN);
	Object::LabelButton::Data data = Object::LabelButton::Create(
		registry,
		resourceStore,
		std::move(transform),
		"PLAY",
		std::move(onClick)
	);

	for (entt::entity entity : data.All())
	{
		registry.emplace<Tag::MainMenu>(entity);
		registry.emplace<Tag::DontDestroyOnLoad>(entity);
	}
}


void Object::SettingsButton::Create(
	entt::registry& registry, 
	GameState& gameState, 
	Nc::ResourceStore& resourceStore
) noexcept
{
	constexpr Nc::Vector2f ANCHOR_POINT = Nc::Vector2f(0.5f, 0.56f);
	constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

	std::function<void()> onClick = [&gameState, &registry]()
	{
		auto view = registry.view<Tag::MainMenu, Component::UI::Transform>();
		for (auto [entity, transform] : view.each()) transform.isVisible = false;
		
		Structure::SettingsMenu::Open(registry, gameState);
	};
	
	Component::UI::Transform transform = Component::UI::Transform(ANCHOR_POINT, ORIGIN);
	Object::LabelButton::Data data = Object::LabelButton::Create(
		registry,
		resourceStore,
		std::move(transform),
		"SETTINGS",
		std::move(onClick)
	);

	for (entt::entity entity : data.All())
	{
		registry.emplace<Tag::MainMenu>(entity);
		registry.emplace<Tag::DontDestroyOnLoad>(entity);
	}
}


void Object::ExitButton::Create(
	entt::registry& registry, 
	GameState& gameState, 
	Nc::ResourceStore& resourceStore
) noexcept
{
	constexpr Nc::Vector2f ANCHOR_POINT = Nc::Vector2f(0.5f, 0.62f);
	constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

	Component::UI::Transform transform = Component::UI::Transform(ANCHOR_POINT, ORIGIN);

	std::function<void()> onClick = [&gameState]()
		{
			gameState.shouldExit = true;
		};

	Object::LabelButton::Data data = Object::LabelButton::Create(
		registry,
		resourceStore,
		std::move(transform),
		"EXIT",
		std::move(onClick)
	);

	for (entt::entity entity : data.All())
	{
		registry.emplace<Tag::MainMenu>(entity);
		registry.emplace<Tag::DontDestroyOnLoad>(entity);
	}
}