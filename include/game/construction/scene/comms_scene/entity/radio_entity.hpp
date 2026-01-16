#pragma once
#include "entt/entity/fwd.hpp"


struct SceneContext;


namespace Entity
{

class Radio final
{
public:
	// ------ Functions ------
	static entt::entity Create(const SceneContext& context) noexcept;

};

}
