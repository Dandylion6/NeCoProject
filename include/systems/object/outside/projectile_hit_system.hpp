#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"
#include <vector>


class ProjectileHitSystem
{
public:
	static void Update(
		entt::registry& registry, float deltaTime
	);

private:
	static void CheckForHits(
		entt::registry& registry,
		std::vector<Nc::Vector2f>& hitPositions
	);

};