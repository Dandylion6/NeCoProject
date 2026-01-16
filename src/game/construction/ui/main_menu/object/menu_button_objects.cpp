#include "game/construction/ui/main_menu/object/menu_button_objects.hpp"

#include <functional>
#include <utility>

#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/game.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "game/state/game_state.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/main_menu_tag.hpp"


void Object::PlayButton::Create(const SceneContext& context, Game& game) noexcept
{
	constexpr Nc::Vector2f ANCHOR_POINT = Nc::Vector2f(0.5f, 0.5f);
	constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

	std::function onClick = [&game, context]()
	{
		game.Load();
		Structure::MainMenu::Close(context);
	};

	Component::UI::Transform transform = Component::UI::Transform(ANCHOR_POINT, ORIGIN);
	const LabelButton::Data data = LabelButton::Create(context, transform, "PLAY", std::move(onClick));

	for (const entt::entity entity : data.All())
	{
		context.registry.emplace<Tag::MainMenu>(entity);
		context.registry.emplace<Tag::DontDestroyOnLoad>(entity);
	}
}


void Object::SettingsButton::Create(const SceneContext& context) noexcept
{
	constexpr Nc::Vector2f ANCHOR_POINT = Nc::Vector2f(0.5f, 0.56f);
	constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

	std::function onClick = [context]()
	{
		const auto view = context.registry.view<Tag::MainMenu, Component::UI::Transform>();
		for (auto [entity, transform] : view.each())
			transform.isVisible = false;

		Structure::SettingsMenu::Open(context);
	};

	Component::UI::Transform transform = Component::UI::Transform(ANCHOR_POINT, ORIGIN);
	const LabelButton::Data data = LabelButton::Create(context, transform, "SETTINGS", std::move(onClick));

	for (const entt::entity entity : data.All())
	{
		context.registry.emplace<Tag::MainMenu>(entity);
		context.registry.emplace<Tag::DontDestroyOnLoad>(entity);
	}
}


void Object::ExitButton::Create(const SceneContext& context) noexcept
{
	constexpr Nc::Vector2f ANCHOR_POINT = Nc::Vector2f(0.5f, 0.62f);
	constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

	std::function onClick = [&gameState = context.game]()
	{
		gameState.shouldExit = true;
	};

	Component::UI::Transform transform = Component::UI::Transform(ANCHOR_POINT, ORIGIN);
	const LabelButton::Data data = LabelButton::Create(context, transform, "EXIT", std::move(onClick));

	for (const entt::entity entity : data.All())
	{
		context.registry.emplace<Tag::MainMenu>(entity);
		context.registry.emplace<Tag::DontDestroyOnLoad>(entity);
	}
}
