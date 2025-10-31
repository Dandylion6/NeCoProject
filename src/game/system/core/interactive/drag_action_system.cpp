#include "components/core/drag_action_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/context/render_context.hpp"
#include "core/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/drag_action_system.hpp"
#include "utility/bounds.hpp"
#include "utility/vector2.hpp"


bool DragActionSystem::Update(
	entt::registry& registry, GameState& gameState, RenderContext& renderContext
)
{
	bool clickReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);

	bool isHovering = false;
	auto view = registry.view<Component::DragAction>();
	for (auto [entity, drag] : view.each())
	{
		if (drag.isTarget)
		{
			Nc::Vector2f mousePosition = GetMousePosition();
			mousePosition -= Nc::Vector2f(renderContext.renderRectangle.x, renderContext.renderRectangle.y);
			mousePosition /= renderContext.renderScale;

			drag.draggedDelta = mousePosition - drag.startPosition;

			if (clickReleased) drag.isTarget = false;
			return false;
		}

		DragResult result = UpdateSceneDrag(registry, gameState, renderContext, entity, drag);
		switch (result)
		{
		case DragActionSystem::Hovering: return true;
		case DragActionSystem::NotHovering:
			return UpdateUiDrag(registry, gameState, renderContext.windowSize);
			break;
		case DragActionSystem::Pressed: return false;
		}
	}
	return isHovering;
}


DragActionSystem::DragResult DragActionSystem::UpdateSceneDrag(
	entt::registry& registry, 
	GameState& gameState,
	RenderContext& renderContext, 
	const entt::entity entity, 
	Component::DragAction& drag
)
{
	const Component::Transform& transform = registry.get<const Component::Transform>(entity);
	if (gameState.currentScene != transform.boundScene) return NotHovering;

	Nc::Vector2f mousePosition = GetMousePosition();
	mousePosition -= Nc::Vector2f(renderContext.renderRectangle.x, renderContext.renderRectangle.y);
	mousePosition /= renderContext.renderScale;

	Nc::Bounds bounds = Nc::Bounds(transform);
	if (!Nc::Bounds::PointInBounds(bounds, mousePosition)) return NotHovering;
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		drag.isTarget = true;
		drag.startPosition = mousePosition;
		return Pressed;
	}
	return Hovering;
}


bool DragActionSystem::UpdateUiDrag(
	entt::registry& registry, GameState& gameState, Nc::Vector2i windowSize
)
{
	// TODO: Add UI functionality.
	return false;
}