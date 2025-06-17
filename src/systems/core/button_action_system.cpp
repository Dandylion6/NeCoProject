#include "components/core/button_action_component.h"
#include "components/core/transform_component.h"
#include "core/game_state.h"
#include "core/render_context.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/button_action_system.h"
#include "utility/bounds.h"
#include "utility/vector2.h"


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
		UpdateButtonDownTime(button, deltaTime);

		if (!button.isActive) continue;
		if (gameState.currentScene != transform.boundScene) continue;

		Nc::Vector2f mousePosition = GetMousePosition();
		mousePosition -= Nc::Vector2f(renderContext.renderRectangle.x, renderContext.renderRectangle.y);
		mousePosition /= renderContext.renderScale;

		Nc::Bounds bounds = Nc::Bounds(transform);
		if (!Nc::Bounds::PointInBounds(bounds, mousePosition)) continue;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (button.isActive)
			{
				button.isActive = false;
				button.onClick();
			}
		}
		else isHovering = true;
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
		Nc::Vector2f mousePosition = GetMousePosition();
		Nc::Bounds bounds = Nc::Bounds(transform, windowSize);
		
		if (!Nc::Bounds::PointInBounds(bounds, mousePosition)) continue;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) button.onClick();
		else isHovering = true;
		break;
	}
	return isHovering;	
}


void ButtonActionSystem::UpdateButtonDownTime(
	Component::ButtonAction& button, float deltaTime
)
{
	if (button.isActive) return;
	button.lastPressedSeconds += deltaTime;
	if (button.lastPressedSeconds >= button.buttonDownTime)
	{
		button.isActive = true;
		button.lastPressedSeconds = 0.0f;
	}
}
