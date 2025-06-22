#pragma once
#include "utility/vector2.h"


namespace Component
{
	struct Artillery
	{
		Nc::Vector2f aimPosition = Nc::Vector2f::Zero();

		Artillery() = default;
	};
}