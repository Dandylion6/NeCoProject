#pragma once
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"


namespace Construct
{
	entt::entity SceneBackgroundEntity(
		Texture2D&& texture, entt::registry& registry, Scene scene
	);
}