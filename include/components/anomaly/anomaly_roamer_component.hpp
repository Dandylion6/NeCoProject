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

		//@brief Targets that determine roamer behaviour.
		enum class Target: uint8_t
		{
			Bunker, // The player bunker.
			Artillery, // The artillery base.
		};

		float speed = 1.2f;
		Behaviour behaviour = Strider;
		Target target = Target::Bunker;
	};
}