#pragma once


namespace Component
{
	struct Strider
	{
		float moveSpeed = 0.1f;

		Strider() = default;
		Strider(float moveSpeed) : moveSpeed(moveSpeed) { };
	};
}