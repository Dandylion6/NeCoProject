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
	static void UpdateBlipStability(entt::registry& registry, Component::Radar& machine, float time);
	static bool ShouldBlipGlitch(Component::Blip& blip, Component::Radar& machine, float secondsSinceLastGlitch, size_t blipCount);
	static float GetDegradationValue(float attractionPercentage);
	static void GlitchBlip(entt::registry& registry, Component::Radar& radar, Component::Blip& blip, const entt::entity entity, float time);
	static void SetRandomGlitchSpawnInterval(Component::Radar& machine);
};