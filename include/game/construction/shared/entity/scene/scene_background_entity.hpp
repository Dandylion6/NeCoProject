#pragma once
#include "raylib.h"
#include "entt/entity/fwd.hpp"
#include "game/state/scene.hpp"


namespace Entity
{

class SceneBackground final
{
public:
	// ------ Functions ------
	static entt::entity Create(entt::registry& registry, const Texture2D& texture, Scene scene) noexcept;

};

}