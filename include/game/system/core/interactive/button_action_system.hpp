#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc { struct RenderContext; };


class ClickSystem
{
public:
	static bool Update(entt::registry& registry, GameState& gameState, Nc::RenderContext& renderContext);

private:
	static bool UpdateSceneButtons(entt::registry& registry, GameState& gameState, Nc::RenderContext& renderContext);
	static bool UpdateUiButtons(entt::registry& registry, GameState& gameState, Nc::Vector2i windowSize);

};