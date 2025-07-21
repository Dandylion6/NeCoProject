#pragma once
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
class ResourceStore;


class TextRenderSystem
{
public:
	static void DrawScreen(
		entt::registry& registry,
		Scene currentScene,
		Nc::Vector2f cameraPosition
	);

	static void DrawUi(
		entt::registry& registry,
		ResourceStore& resourceStore,
		Nc::Vector2f windowSize
	);

};