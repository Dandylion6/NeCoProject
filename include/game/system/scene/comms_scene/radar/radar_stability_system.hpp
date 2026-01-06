#pragma once
#include "entt/entity/fwd.hpp"

struct AnomalyState;
struct GameState;
namespace Component
{
	struct Blip;
	struct Radar;
}


class RadarStabilitySystem
{
public:
	static void Update(
		entt::registry& registry, 
		GameState& gameState, 
		float time, 
		float deltaTime
	) noexcept;

	static void Restart(entt::registry& registry, entt::entity entity) noexcept;

private:
	static void UpdateBlipStability(
		entt::registry& registry, 
		AnomalyState& anomalyState, 
		Component::Radar& machine, 
		float time
	) noexcept;

	static bool ShouldBlipGlitch(
		Component::Blip& blip, 
		Component::Radar& machine, 
		float secondsSinceLastGlitch, 
		size_t blipCount
	) noexcept;

	static float GetDegradationValue(float attractionPercentage) noexcept;

	static void GlitchBlip(
		entt::registry& registry, 
		Component::Radar& radar, 
		Component::Blip& blip, 
		entt::entity entity, 
		float time
	) noexcept;

	static void SetRandomGlitchSpawnInterval(Component::Radar& machine) noexcept;
};