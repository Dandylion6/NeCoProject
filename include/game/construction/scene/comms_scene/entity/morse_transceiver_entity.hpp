#pragma once
#include "entt/entity/fwd.hpp"


struct SceneContext;


namespace Entity
{

class MorseTransceiver final
{
public:
	static entt::entity Create(const SceneContext& context) noexcept;

};

}
