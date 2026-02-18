#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"


class SpriteRenderSystem
{
public:
	static void DrawScreen(
	    const Shader& lightShader,
		entt::entity entity,
		entt::registry& registry,
		Nc::Vector2f cameraPosition
	);

	static void DrawUi(
		entt::entity entity,
		entt::registry& registry,
		Nc::Vector2f windowSize
	);

};