#pragma once
#include "core/render_context.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"


class RenderingSystem
{
public:
	static void Draw(
		entt::registry& registry,
		RenderContext& renderContext,
		Scene currentScene
	);

private:
	enum RenderType
	{
		Ignore,
		Sprite,
		Text
	};

	static RenderType GetRenderType(
		entt::registry& registry, 
		const entt::entity entity,
		Scene currentScene
	);
};