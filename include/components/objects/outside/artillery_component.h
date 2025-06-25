#pragma once
#include "utility/vector2.h"


namespace Component
{
	struct Artillery
	{
		Nc::Vector2f aimPosition = Nc::Vector2f(180.0f, 180.0f);
		Nc::Vector2f targetPosition = Nc::Vector2f(180.0f, 180.0f);
		float aimStartupDelay = 0.0f;
		float fireDelay = 0.0f;
		bool isReadyToFire = true;
		bool receivedFireRequest = false;

		Artillery() = default;
	};
}