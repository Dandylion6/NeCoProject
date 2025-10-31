#include "game/component/core/interactive/button_action_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "core/runtime/render_context.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/core/interactive/button_action_system.hpp"
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"


bool ButtonActionSystem::Update(
	entt::registry& registry, GameState& gameState, RenderContext& renderContext
)
{
	bool isHovering = UpdateUiButtons(registry, gameState, renderContext.windowSize);
	if (!isHovering) isHovering = UpdateSceneButtons(registry, gameState, renderContext);
	return isHovering;
}


bool ButtonActionSystem::UpdateSceneButtons(
	entt::registry& registry, GameState& gameState, RenderContext& renderContext
)
{
	if (gameState.isPaused) return false;

	bool isHovering = false;
	auto view = registry.view<const Component::Transform, Component::ButtonAction>();
	for (auto [entity, transform, button] : view.each())
	{
		if (!button.isActive) continue;
		if (gameState.currentScene != transform.boundScene) continue;

		Nc::Vector2f mousePosition = GetMousePosition();
		mousePosition -= Nc::Vector2f(renderContext.renderRectangle.x, renderContext.renderRectangle.y);
		mousePosition /= renderContext.renderScale;

		Nc::Bounds bounds = Nc::Bounds(transform);
		if (!Nc::Bounds::PointInBounds(bounds, mousePosition)) continue;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			button.isActive = false;
			button.onClick();
		}
		else isHovering = true;
	}
	return isHovering;
}


bool ButtonActionSystem::UpdateUiButtons(
	entt::registry& registry, GameState& gameState, Nc::Vector2i windowSize
)
{
	bool isHovering = false;
	auto view = registry.view<const Component::UI::Transform, Component::ButtonAction>();
	for (auto [entity, transform, button] : view.each())
	{
		button.isActive = transform.isVisible;
		if (!button.isActive) continue;

		Nc::Vector2f mousePosition = GetMousePosition();
		Nc::Bounds bounds = Nc::Bounds(transform, windowSize);
		
		if (!Nc::Bounds::PointInBounds(bounds, mousePosition)) continue;
		if (button.isActive)
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				button.isActive = false;
				button.onClick();
			}
			else isHovering = true;
		}
		break;
	}
	return isHovering;	
}
