#pragma once
#include "entt/entity/fwd.hpp"
struct AnomalyState;


class RadarStabilitySystem
{
public:
	static void Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime);
};