#include "assemblers/scenes/settings_menu/settings_background_entity.hpp"
#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "components/core/input_component.hpp"
#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/rendering/sprite_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/scene/dont_destroy_on_load_tag.hpp"
#include "components/ui/settings_tag.hpp"
#include "components/ui/toggle_state_component.hpp"
#include "core/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/color_palette.hpp"
#include "utility/vector2.hpp"
#include <functional>
#include <utility>


namespace Construct 
{
	static const entt::entity SettingsBackdropEntity(
		entt::registry& registry, Nc::Vector2f windowSize
	)	
	{
		const entt::entity entity = registry.create();

		Nc::Vector2f center = Nc::Vector2f::Scale(0.5f);

		registry.emplace<Tag::DontDestroyOnLoad>(entity);
		registry.emplace<Tag::Settings>(entity);
		registry.emplace<Component::UiTransform>(entity, center, center, windowSize);
		registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);

		return entity;
	};
};


const entt::entity Construct::SettingsBackgroundEntity(
	entt::registry& registry, GameState& gameState, Nc::Vector2f windowSize
)
{
	Construct::SettingsBackdropEntity(registry, windowSize);

    const entt::entity entity = registry.create();

    Texture2D texture = LoadTexture("assets/environment/backgrounds/main_menu.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);
	Nc::Vector2f center = Nc::Vector2f::Scale(0.5f);

	registry.emplace<Tag::DontDestroyOnLoad>(entity);
	registry.emplace<Tag::Settings>(entity);
	registry.emplace<Component::ToggleState>(entity);
	registry.emplace<Component::UiTransform>(entity, center, center, size, Nc::Vector2f::Zero(), 1);
	registry.emplace<Component::Sprite>(entity, std::move(texture), 0.4f);

	std::function<void()> toggleSettings = [&registry, &gameState]()
	{
		SettingsMenu::Toggle(registry, gameState);
	};

	registry.emplace<Component::Input>(entity, KEY_ESCAPE, std::move(toggleSettings));
    return entity;
};