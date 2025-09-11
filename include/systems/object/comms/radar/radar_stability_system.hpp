#pragma once
#include "components/objects/comms/radar.hpp"
#include "entt/entity/fwd.hpp"
struct AnomalyState;


class RadarStabilitySystem
{
public:
	static void Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime);

private:
	static void UpdateStability(entt::registry& registry, AnomalyState& anomalyState, float deltaTime);
	static void UpdateBlipStability(entt::registry& registry, Component::RadarMachine& machine);

};