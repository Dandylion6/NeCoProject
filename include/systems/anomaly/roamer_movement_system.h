#pragma once
#include "entt/entity/fwd.hpp"


class RoamerMovementSystem
{
public:
	static void Update(
		entt::registry& registry, 
		float deltaTime
	);

};