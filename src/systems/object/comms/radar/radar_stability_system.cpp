#include "components/objects/comms/radar.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"
#include "systems/object/comms/radar/radar_stability_system.hpp"
#include <cmath>
#include <cstdint>


void RadarStabilitySystem::Update(entt::registry& registry, AnomalyState& anomalyState, float time, float deltaTime)
{
	constexpr float DEGRADATION_FACTOR = 9.0f;
	constexpr float DEGRADATION_CURVE = 1.4f;

	auto view = registry.view<Component::RadarMachine>();
	for (auto [entity, machine] : view.each())
	{
		if (!machine.isActive) continue;

		if (anomalyState.attractionPercentage >= AnomalyState::DEGRADATION_THRESHOLD)
		{
			float adjustedPercentage = anomalyState.attractionPercentage - AnomalyState::DEGRADATION_THRESHOLD;
			float curve = DEGRADATION_FACTOR * std::powf(DEGRADATION_CURVE, adjustedPercentage * AnomalyState::PRECENTAGE_FACTOR);
			float degredation = curve * 0.016f;
			machine.sability -= degredation * deltaTime;
		}

		UpdateBlipStability(registry, machine, time);
	}
}


void RadarStabilitySystem::UpdateBlipStability(entt::registry& registry, Component::RadarMachine& machine, float time)
{
	bool isStable = machine.sability >= 80.0f;
	float secondsSinceLastGlitch = time - machine.lastGlitchTime;

	uint32_t blipCount = 0u;
	auto view = registry.view<Component::Blip>();
	for (auto [entity, blip] : view.each())
	{
		if (isStable) blip.coordState = Component::Blip::CoordTextState::Stable;
		++blipCount;
	}

	if (isStable) return;
	
	uint32_t blipIndex = 0u;
	for (auto [entity, blip] : view.each())
	{
		if (!ShouldBlipGlitch(blip, machine, secondsSinceLastGlitch, blipIndex++, blipCount)) continue;
		SetRandomGlitchSpawnInterval(machine);
		
		blip.coordState = Component::Blip::CoordTextState::CoordinateJumble;
		// TODO: Add more glitch logic.
	}
}


bool RadarStabilitySystem::ShouldBlipGlitch(
	Component::Blip& blip, Component::RadarMachine& machine, float secondsSinceLastGlitch, uint32_t blipIndex, uint32_t blipCount
)
{
	if (secondsSinceLastGlitch <= machine.nextGlitchSpawnSeconds) return false;
	if (blip.coordState != Component::Blip::CoordTextState::Stable) return false;
	
	float chance = static_cast<float>(blipIndex) / static_cast<float>(blipCount);
	float deterministicValue = static_cast<float>(GetRandomValue(0, 100)) * 0.01f;
	if (chance < deterministicValue) return false;

	return true;
}


void RadarStabilitySystem::SetRandomGlitchSpawnInterval(Component::RadarMachine& machine)
{
	constexpr Nc::Vector2f BASE_GLITCH_SPAWN_RANGE = Nc::Vector2f(1.0f, 3.0f); // The base interval range for new glitches to appear. Measured in minutes.

	float minutesToNextGlitch = GetRandomValue(static_cast<int>(BASE_GLITCH_SPAWN_RANGE.x * 50.0f), static_cast<int>(BASE_GLITCH_SPAWN_RANGE.y * 50.0f)) * 0.02f;
	// TODO: Add intensity scaling based intervals.

	machine.nextGlitchSpawnSeconds = minutesToNextGlitch * 60.0f;
}