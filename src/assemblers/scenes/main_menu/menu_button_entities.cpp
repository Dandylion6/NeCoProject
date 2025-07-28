#include "assemblers/scenes/main_menu/menu_button_entities.hpp"
#include "assemblers/ui/label_button_object.hpp"
#include "components/core/button_action_component.hpp"
#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/ui/main_menu_tag.hpp"
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"
#include <functional>
#include <string>
#include <utility>


void Construct::PlayButtonObject(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
)
{
	Component::UiTransform transform = Component::UiTransform(
		Nc::Vector2f(0.5f, 0.5f), Nc::Vector2f::Scale(0.5f)
	);
	
	std::function<void()> onClick = [&gameState, &registry]()
	{
		auto view = registry.view<Tag::MainMenu, Component::UiTransform>();
		for (auto [entity, transform] : view.each()) transform.isVisible = false;
		
		gameState.currentScene = CommsRoom;
		gameState.isPaused = false;
	};
	 
	LabelButton button = Construct::LabelButtonObject(
		std::move(transform), "PLAY", std::move(onClick), registry, resourceStore
	);
	
	registry.emplace<Tag::MainMenu>(button.button);
	registry.emplace<Tag::MainMenu>(button.label);
}


void Construct::ExitButtonObject(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
)
{
	Component::UiTransform transform = Component::UiTransform(
		Nc::Vector2f(0.5f, 0.56f), Nc::Vector2f::Scale(0.5f)
	);
	
	std::function<void()> onClick = [&gameState]()
	{
		gameState.shouldExit = true;
	};
	 
	LabelButton button = Construct::LabelButtonObject(
		std::move(transform), "EXIT", std::move(onClick), registry, resourceStore
	);
	
	registry.emplace<Tag::MainMenu>(button.button);
	registry.emplace<Tag::MainMenu>(button.label);
}