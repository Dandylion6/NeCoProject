#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
struct SceneContext;


namespace Entity
{

class Projectile final
{
public:
	// ------ Functions ------
	static entt::entity Create(const SceneContext& context, Nc::Vector2f hitPosition) noexcept;
};

}
