#pragma once
#include "entt/entity/fwd.hpp"
struct AnomalyState;


class MachineSystem
{
public:
	static void Update(
		entt::registry& registry, AnomalyState& anomalyState, float deltaTime
	);
};