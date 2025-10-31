#pragma once
#include "utility/vector2.hpp"


namespace Component
{
	struct Projectile
	{
		Nc::Vector2f hitPosition = Nc::Vector2f::Zero();
		float travelTimeLeft = 0.0f;
		bool isActive = true;

		Projectile(
			Nc::Vector2f hitPosition,
			float travelTimeLeft
		):
			hitPosition(hitPosition),
			travelTimeLeft(travelTimeLeft)
		{ };
	};
}