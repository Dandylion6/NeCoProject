#pragma once
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
struct GameState;
struct RenderContext;


class ButtonActionSystem
{
public:
	static bool Update(entt::registry& registry, GameState& gameState, RenderContext& renderContext);

private:
	static bool UpdateSceneButtons(entt::registry& registry, GameState& gameState, RenderContext& renderContext);
	static bool UpdateUiButtons(entt::registry& registry, GameState& gameState, Nc::Vector2i windowSize);

};