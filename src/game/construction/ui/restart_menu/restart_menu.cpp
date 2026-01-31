#include "game/construction/ui/restart_menu/restart_menu.hpp"

#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/game.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/restart_menu/object/restart_background_object.hpp"
#include "game/construction/ui/restart_menu/object/restart_button_objects.hpp"
#include "game/state/game_state.hpp"
#include "game/tag/ui/restart_menu_tag.hpp"


void Structure::RestartMenu::Build(const SceneContext& context, Game &game, const Nc::Vector2f windowSize) noexcept
{
	Object::RestartMenuBackground::Create(context, windowSize);
	Object::RestartButton::Create(context, game);
	Object::RestartToMainButton::Create(context);
	Close(context);
}

void Structure::RestartMenu::Open(const SceneContext& context) noexcept { Toggle(context, true); }

void Structure::RestartMenu::Close(const SceneContext& context) noexcept { Toggle(context, false); }

void Structure::RestartMenu::Toggle(const SceneContext& context, const bool active) noexcept
{
	context.game.isPaused = active;

	const auto view = context.registry.view<const Tag::RestartMenu, Component::UI::Transform>();
	for (auto [entity, transform] : view.each())
		transform.isVisible = active;
}