#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
struct GameState;


namespace Construct
{
	const entt::entity RestartMenuBackgroundEntity(
		entt::registry& registry, GameState& gameState, Nc::Vector2f windowSize
	);
}