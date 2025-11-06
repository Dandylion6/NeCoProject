#pragma once
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
namespace Nc { class ResourceStore; };


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
		Nc::ResourceStore& resourceStore, 
		Nc::Vector2f windowSize
	);

};