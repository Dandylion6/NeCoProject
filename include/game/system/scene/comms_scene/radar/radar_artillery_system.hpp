#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.hpp"


class RadarArtillerySystem
{
public:
	static void Update(
		entt::registry& registry, 
		float deltaTime
	);

private:
	static Nc::Vector2f GetArtilleryAimPosition(entt::registry& registry);

};