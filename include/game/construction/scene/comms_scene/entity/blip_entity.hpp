#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
struct SceneContext;


namespace Entity
{

class Blip final
{
public:
	// ------ Functions ------
	static entt::entity Create(const SceneContext& context, Nc::Vector2f position, int16_t health = 10) noexcept;

};

}