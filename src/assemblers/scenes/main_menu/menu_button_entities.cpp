#include "assemblers/scenes/main_menu/menu_button_entities.h"
#include "components/core/button_action_component.h"
#include "components/core/rendering/rectangle_component.h"
#include "components/core/rendering/text_component.h"
#include "components/core/transform_component.h"
#include "components/ui/main_menu_tag.h"
#include "core/game_state.h"
#include "core/render_context.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/color.h"
#include "utility/vector2.h"
#include <functional>
#include <utility>


const entt::entity Construct::PlayButtonEntity(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
)
{
	const entt::entity entity = registry.create();

	registry.emplace<Tag::MainMenu>(entity);
	registry.emplace<Component::UiTransform>(
		entity, Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f(440.0f, 90.0f)
	);
	registry.emplace<Component::Rectangle>(entity, RenderContext::BACKGROUND_COLOR);

	constexpr Nc::Hex TEXT_COLOR = 0x7cff3cff;
	
	registry.emplace<Component::Text>(entity, "PLAY", TEXT_COLOR, WDXL, 64, Component::Text::Center, 8u);

	std::function<void()> onClick = [&gameState, &registry]()
	{
		gameState.currentScene = CommsRoom;
		gameState.isPaused = false;
		
		auto view = registry.view<Tag::MainMenu>();
		for (const entt::entity entity : view)
		{
			registry.destroy(entity);
		}
	};
	registry.emplace<Component::ButtonAction>(entity, std::move(onClick));

	return entity;
}