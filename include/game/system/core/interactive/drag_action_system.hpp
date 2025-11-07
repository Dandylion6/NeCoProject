#pragma once
#include "game/component/core/interactive/drag_action_component.hpp"
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
struct GameState;
namespace Nc { struct RenderContext; };


struct DragActionSystem
{
public:
	static bool Update(entt::registry& registry, GameState& gameState, Nc::RenderContext& renderContext);

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
		Nc::RenderContext& renderContext, 
		const entt::entity entity, 
		Component::Action::Drag& drag
	);
	static bool UpdateUiDrag(entt::registry& registry, GameState& gameState, Nc::Vector2i windowSize);

};