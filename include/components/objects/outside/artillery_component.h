#pragma once
#include "utility/vector2.h"


namespace Component
{
	struct Artillery
	{
		Nc::Vector2f aimPosition = Nc::Vector2f(180.0f, 120.0f);
		float aimingDelay = 0.0f;

		Artillery() = default;
	};
}