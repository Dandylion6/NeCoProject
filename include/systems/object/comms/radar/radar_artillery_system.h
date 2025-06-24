#pragma once
#include "entt/entity/fwd.hpp"
#include "utility/vector2.h"


class RadarArtillerySystem
{
public:
	static void Update(entt::registry& registry, float deltaTime);

private:
	static Nc::Vector2f GetArtilleryAimPosition(
		Nc::Vector2f currentPosition, entt::registry& registry, float deltaTime
	);

};