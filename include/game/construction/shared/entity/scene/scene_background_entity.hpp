#pragma once
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"


namespace Construct
{
	const entt::entity SceneBackgroundEntity(
		Texture2D&& texture, entt::registry& registry, Scene scene
	);
}