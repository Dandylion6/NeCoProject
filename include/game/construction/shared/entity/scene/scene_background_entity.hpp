#pragma once
#include "entt/entity/fwd.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"


namespace Entity
{

class SceneBackground final
{
public:
	static entt::entity Create(
		Texture2D&& texture, 
		entt::registry& registry, 
		Scene scene
	) noexcept;

};

}