#pragma once
#include "components/objects/comms/radar.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "entt/entity/fwd.hpp"
struct AnomalyState;


class RadarStabilitySystem
{
public:
	static void Update(entt::registry& registry, AnomalyState& anomalyState, float time, float deltaTime);

private:
	static void UpdateBlipStability(entt::registry& registry, Component::RadarMachine& machine, float time);
	static bool ShouldBlipGlitch(Component::Blip& blip, Component::RadarMachine& machine, float secondsSinceLastGlitch, uint32_t blipIndex, uint32_t blipCount);
	static void SetRandomGlitchSpawnInterval(Component::RadarMachine& machine);
};