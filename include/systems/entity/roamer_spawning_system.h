#pragma once
#include "entt/entity/fwd.hpp"


class RoamerSpawningSystem
{
public:
	static void Update(
		entt::registry& registry, 
		float deltaTime
	);

	static void SpawnRoamer(entt::registry& registry);

};