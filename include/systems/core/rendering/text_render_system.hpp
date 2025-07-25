#pragma once
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
class ResourceStore;


class TextRenderSystem
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
		ResourceStore& resourceStore, 
		Nc::Vector2f windowSize
	);

};