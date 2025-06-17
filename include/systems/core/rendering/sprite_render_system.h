#pragma once
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.h"


class SpriteRenderSystem
{
public:
	static void DrawScreen(
		entt::registry& registry,
		Scene currentScene, 
		Nc::Vector2f cameraPosition
	);

	static void DrawUI(
		entt::registry& registry,
		Nc::Vector2i windowSize
	);

};