#pragma once
#include "core/data/vector2.hpp"


namespace Component
{
	struct Artillery
	{
		Nc::Vector2f targetPosition = Nc::Vector2f(0.0f, 0.0f);
		Nc::Vector2f aimPosition = targetPosition;
		float aimStartupDelay = 0.0f;
		float fireDelay = 0.0f;
		bool isReadyToFire = true;
		bool receivedFireRequest = false;

		Artillery() = default;
	};
}