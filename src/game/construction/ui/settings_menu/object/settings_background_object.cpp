#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/input_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/settings_menu/object/settings_background_object.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/state/game_state.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/settings_tag.hpp"
#include "game/utility/color_palette.hpp"
#include "raylib.h"
#include <functional>
#include <utility>


void Object::SettingsBackground::Create(
	entt::registry& registry, 
	GameState& gameState, 
	Nc::Vector2f windowSize
) noexcept
{
	Backdrop::Create(registry, windowSize);
	FrontTexture::Create(registry, gameState);
}


const entt::entity Object::SettingsBackground::FrontTexture::Create(
	entt::registry& registry, GameState& gameState
) noexcept
{
	constexpr Nc::Vector2f CENTER = Nc::Vector2f::Scale(0.5);

	const entt::entity entity = registry.create();

	registry.emplace<Tag::Settings>(entity);
	registry.emplace<Tag::DontDestroyOnLoad>(entity);

	Texture2D texture = LoadTexture("assets/environment/backgrounds/main_menu.png");
	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);

	registry.emplace<Component::UI::Transform>(entity, CENTER, CENTER, size, Nc::Vector2f::Zero(), 1);
	registry.emplace<Component::Sprite>(entity, std::move(texture), 0.4f);

	std::function<void()> toggleSettings = [&registry, &gameState]()
	{
		Structure::SettingsMenu::Toggle(registry, gameState);
	};
	registry.emplace<Component::Action::Input>(entity, std::move(toggleSettings), KEY_ESCAPE);
	registry.emplace<Component::Action::Toggle>(entity);

	return entity;
}


const entt::entity Object::SettingsBackground::Backdrop::Create(
	entt::registry& registry, Nc::Vector2f windowSize
) noexcept
{
	constexpr Nc::Vector2f CENTER = Nc::Vector2f::Scale(0.5f);

	const entt::entity entity = registry.create();

	registry.emplace<Tag::DontDestroyOnLoad>(entity);
	registry.emplace<Tag::Settings>(entity);
	registry.emplace<Component::UI::Transform>(entity, CENTER, CENTER, windowSize);
	registry.emplace<Component::Rectangle>(entity, Palette::BACKGROUND_COLOR);

	return entity;
}
