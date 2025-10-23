#pragma once
#include <cstdint>


namespace Component
{
	struct AnomalyRoamer
	{
		enum Behaviour: uint8_t
		{
			Strider, // A roamer that moves stright to a point of interest at a constant speed.
			Phaser, // A roamer that teleports between 2 set points that get closer to the target.
			Phantom, // A roamer that moves fast when hidden but dissapears when close to the target completely.
			BEHAVIOUR_COUNT // Used for iteration and random selection.
		};

		//@brief Targets that determine roamer behaviour.
		enum class Target: uint8_t
		{
			Bunker, // The player bunker.
			Artillery, // The artillery base.
			TARGET_COUNT // Used for iteration and random selection.
		};


		Behaviour behaviour = Strider;
		Target target = Target::Bunker;

		AnomalyRoamer(Behaviour behaviour, Target target) : behaviour(behaviour), target(target) { };
	};
}