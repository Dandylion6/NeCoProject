#pragma once
#include "components/core/drag_action_component.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct GameState;
struct RenderContext;


struct DragActionSystem
{
public:
	static bool Update(entt::registry& registry, GameState& gameState, RenderContext& renderContext);

private:
	enum DragResult
	{
		Hovering,
		NotHovering,
		Pressed
	};

	static DragResult UpdateSceneDrag(
		entt::registry& registry,
		GameState& gameState,
		RenderContext& renderContext, 
		const entt::entity entity, 
		Component::DragAction& drag
	);
	static bool UpdateUiDrag(entt::registry& registry, GameState& gameState, Nc::Vector2i windowSize);

};