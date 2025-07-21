#pragma once
#include "components/core/button_action_component.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct GameState;
struct RenderContext;


class ButtonActionSystem
{
public:
	static void Update(
		entt::registry& registry, 
		GameState& gameState, 
		RenderContext& renderContext,
		float deltaTime
	);

private:
	static bool UpdateSceneButtons(
		entt::registry& registry, 
		GameState& gameState, 
		RenderContext& renderContext,
		float deltaTime
	);

	static bool UpdateUiButtons(
		entt::registry& registry, 
		GameState& gameState, 
		Nc::Vector2i windowSize,
		float deltaTime
	);

	static void UpdateButtonDownTime(
		Component::ButtonAction& button,
		float deltaTime
	);

};