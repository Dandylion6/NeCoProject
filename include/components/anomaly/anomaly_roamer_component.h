#pragma once
#include <cstdint>


namespace Component
{
	struct AnomalyRoamer
	{
		enum Behaviour: uint8_t
		{
			Strider, // A roamer that moves stright to a point of interest.
		};

		Behaviour behaviour = Strider;
		float speed = 1.2f;
	};
}