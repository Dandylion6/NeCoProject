#pragma once
#include "entt/entity/fwd.hpp"
struct SystemContext;
struct AnomalyState;


namespace Component
{
struct Blip;
struct Radar;
}


namespace System::Radar
{

class Stability final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context, AnomalyState& anomaly);
	static void Restart(entt::registry& registry, entt::entity entity);

private:
	// ------ Functions ------
	static void CheckBreakdown(entt::registry& registry, Component::Radar& radar);
	static void UpdateBlipStability(
		const SystemContext& context,
		AnomalyState& anomaly,
		Component::Radar& radar
	);

	static bool ShouldBlipGlitch(entt::registry& registry, size_t blipCount);
	static float GetDegradationValue(float attractionPercentage);
	static void GlitchBlip(
		const SystemContext& context,
		const Component::Radar& radar,
		Component::Blip& blip,
		entt::entity entity
	);

	static void SetRandomGlitchSpawnInterval(entt::registry& registry, Component::Radar& radar);
};

}

