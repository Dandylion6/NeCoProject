#include "game/system/core/interactive/drag_action_system.hpp"

#include "raylib.h"
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"
#include "game/utility/rendering.hpp"


void System::Action::Drag::Update(const SystemContext& context, const Nc::RenderContext& renderContext)
{
	const bool clickPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	const bool clickReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);

	const auto view = context.registry.view<Component::Action::Drag>();
	for (auto [entity, drag] : view.each())
	{
		if (drag.isTarget)
		{
			// TODO: Separate for UI.
			const Nc::Vector2f mousePosition = Renderer::GetWorldPosition(renderContext, GetMousePosition());
			drag.draggedDelta = mousePosition - drag.startPosition;

			if (clickReleased) drag.isTarget = false;
			return;
		}

		switch (UpdateSceneDrag(context, renderContext, entity, drag, clickPressed))
		{
		case Hovering: return Nc::Cursor::AssignIfHigherPriority(context.game.cursor, Nc::Cursor::Grab);
		case NotHovering: return UpdateUiDrag(context, renderContext.windowSize, clickPressed);
		case Pressed: return;
		}
	}
}


System::Action::Drag::Result System::Action::Drag::UpdateSceneDrag(
	const SystemContext& context,
	const Nc::RenderContext& renderContext,
	const entt::entity entity,
	Component::Action::Drag& drag,
	const bool clickPressed
)
{
	const auto& transform = context.registry.get<const Component::Transform>(entity);
	if (context.game.currentScene != transform.boundScene) return NotHovering;

	const Nc::Vector2f mousePosition = Renderer::GetWorldPosition(renderContext, GetMousePosition());

	const auto bounds = Nc::Bounds(transform);
	if (!Nc::Bounds::PointInBounds(bounds, mousePosition - renderContext.cameraPosition)) return NotHovering;
	if (clickPressed)
	{
		drag.isTarget = true;
		drag.startPosition = mousePosition;
		return Pressed;
	}
	return Hovering;
}


void System::Action::Drag::UpdateUiDrag(const SystemContext& context, Nc::Vector2i windowSize, const bool clickPressed)
{
	// TODO: Add UI functionality.
}
