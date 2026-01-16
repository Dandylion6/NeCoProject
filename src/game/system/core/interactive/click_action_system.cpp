#include "game/system/core/interactive/click_action_system.hpp"

#include "raylib.h"
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"


void System::Action::Click::Update(const SystemContext& context, const Nc::RenderContext& renderContext)
{
	UpdateUiButtons(context, renderContext.windowSize);
	if (context.game.cursor == Nc::Cursor::Standard)
		UpdateSceneButtons(context, renderContext);
}


void System::Action::Click::UpdateSceneButtons(const SystemContext& context, const Nc::RenderContext& renderContext)
{
	if (context.game.isPaused) return;

	const auto view = context.registry.view<const Component::Transform, Component::Action::Click>();
	for (auto [entity, transform, button] : view.each())
	{
		if (!button.isActive) continue;
		if (context.game.currentScene != transform.boundScene) continue;

		auto mousePosition = Nc::Vector2f(GetMousePosition());
		mousePosition -= Nc::Vector2f(renderContext.renderRectangle.x, renderContext.renderRectangle.y);
		mousePosition /= renderContext.renderScale;

		const auto bounds = Nc::Bounds(transform);
		if (!Nc::Bounds::PointInBounds(bounds, mousePosition)) continue;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			button.isActive = false;
			button.onClick();
		} else Nc::Cursor::AssignIfHigherPriority(context.game.cursor, Nc::Cursor::Clickable);
		break; // Already a target, no need to check further.
	}
}


void System::Action::Click::UpdateUiButtons(const SystemContext& context, const Nc::Vector2i windowSize)
{
	const auto view = context.registry.view<const Component::UI::Transform, Component::Action::Click>();
	for (auto [entity, transform, button] : view.each())
	{
		button.isActive = transform.isVisible;
		if (!button.isActive) continue;

		const auto mousePosition = Nc::Vector2f(GetMousePosition());
		const auto bounds = Nc::Bounds(transform, windowSize);

		if (!Nc::Bounds::PointInBounds(bounds, mousePosition)) continue;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			button.isActive = false;
			button.onClick();
		} else Nc::Cursor::AssignIfHigherPriority(context.game.cursor, Nc::Cursor::Clickable);
		break; // Already a target, no need to check further.
	}
}
