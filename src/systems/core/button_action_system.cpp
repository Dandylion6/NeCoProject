#include "components/core/button_action_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/button_action_system.hpp"
#include "utility/bounds.hpp"
#include "utility/vector2.hpp"


void ButtonActionSystem::Update(
	entt::registry& registry, 
	GameState& gameState, 
	RenderContext& renderContext,
	float deltaTime
)
{
	bool isHovering = UpdateUiButtons(registry, gameState, renderContext.windowSize, deltaTime);
	if (!isHovering) isHovering = UpdateSceneButtons(registry, gameState, renderContext, deltaTime);
	
	MouseCursor cursor = isHovering ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT;
	SetMouseCursor(cursor);
}


bool ButtonActionSystem::UpdateSceneButtons(
	entt::registry& registry, 
	GameState& gameState, 
	RenderContext& renderContext,
	float deltaTime
)
{
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


bool ButtonActionSystem::UpdateUiButtons(
	entt::registry& registry, 
	GameState& gameState, 
	Nc::Vector2i windowSize,
	float deltaTime
)
{
	bool isHovering = false;
	auto view = registry.view<const Component::UiTransform, Component::ButtonAction>();
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
