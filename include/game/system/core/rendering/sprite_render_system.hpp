#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"


class SpriteRenderSystem
{
public:
	static void DrawScreen(
		const entt::entity entity,
		entt::registry& registry,
		Nc::Vector2f cameraPosition
	);

	static void DrawUi(
		const entt::entity entity,
		entt::registry& registry,
		Nc::Vector2f windowSize
	);

};