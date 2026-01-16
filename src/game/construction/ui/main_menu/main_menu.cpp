#include "game/construction/ui/main_menu/main_menu.hpp"

#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/main_menu/entity/menu_background_entity.hpp"
#include "game/construction/ui/main_menu/object/menu_button_objects.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/tag/ui/main_menu_tag.hpp"


void Structure::MainMenu::Build(const SceneContext& context, Game& game) noexcept
{
	Object::PlayButton::Create(context, game);
	Object::SettingsButton::Create(context);
	Object::ExitButton::Create(context);

	Entity::MainMenuBackground::Create(context);

	Close(context);
}


void Structure::MainMenu::Open(const SceneContext& context) noexcept
{
	const auto view = context.registry.view<Tag::MainMenu, Component::UI::Transform>();
	for (auto [entity, transform] : view.each())
		transform.isVisible = true;

	context.game.currentScene = NullScene;
	context.game.isPaused = true;
}


void Structure::MainMenu::Close(const SceneContext& context) noexcept
{
	const auto view = context.registry.view<Tag::MainMenu, Component::UI::Transform>();
	for (auto [entity, transform] : view.each())
		transform.isVisible = false;

	context.game.isPaused = false;
}
