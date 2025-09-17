#include "components/objects/comms/radar.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/radar/blip_glitch_system.hpp"
#include "utility/vector2.hpp"
#include "systems/object/comms/radar/radar_stability_system.hpp"
#include <cmath>
#include <cstdint>
#ifdef  DEBUG_BUILD
#include "core/game.hpp"
#include "core/debug_context.hpp"
#endif


void RadarStabilitySystem::Update(entt::registry& registry, AnomalyState& anomalyState, float time, float deltaTime)
{
	constexpr float DEGRADATION_FACTOR = 9.0f;
	constexpr float DEGRADATION_CURVE = 1.4f;

	auto view = registry.view<Component::RadarMachine>();
	for (auto [entity, machine] : view.each())
	{
#ifdef DEBUG_BUILD
		Game::debugContext.radarStabilityPercentage = machine.sability;
		if (IsKeyPressed(KEY_K)) machine.sability = std::fminf(machine.sability + 10.0f, 100.0f);
		if (IsKeyPressed(KEY_L)) machine.sability = std::fmaxf(machine.sability - 10.0f, 0.0f);
#endif

		if (!machine.isActive) continue;
		if (machine.nextGlitchSpawnSeconds == 0.0f) SetRandomGlitchSpawnInterval(machine);

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
	bool isStable = machine.sability >= Component::RadarMachine::STABLE_LEVEL;
	float secondsSinceLastGlitch = time - machine.lastGlitchTime;

	uint8_t blipCount = 0u;
	uint8_t glitchCount = 0u;

	auto view = registry.view<Component::Blip>();
	for (auto [entity, blip] : view.each())
	{
		if (isStable) blip.state = Component::Blip::Stable;
		if (blip.state != Component::Blip::Stable) ++glitchCount;
		++blipCount;
	}

	machine.glitchCount = glitchCount; // Keeps glictch count up to date.

	if (isStable) return;
	
	uint8_t blipIndex = 0u;
	for (auto [entity, blip] : view.each())
	{
		if (!ShouldBlipGlitch(blip, machine, secondsSinceLastGlitch, blipIndex++, blipCount)) continue;
		SetRandomGlitchSpawnInterval(machine);
		
		// Glitch spawning logic:
		// - If radar stability < 80%, glitches can occur.
		// - Check if enough time has passed since the last glitch (interval scales with stability).
		// - For each stable blip:
		//     - Use weighted random selection for glitch severity based on current stability:
		//         - 50–80%: Only jumble glitches.
		//         - 20–50%: Mostly jumble, some text errors.
		//         - 0–20%: Jumble, text errors, and complete failures.
		//     - Scale glitch duration by severity and stability (lower stability = longer duration).
		// - After glitching, reset the glitch spawn timer.

		if (machine.sability >= Component::RadarMachine::HEALTHY_LEVEL)
		{
			// Jumble
		}
		else if (machine.sability >= Component::RadarMachine::UNSTABLE_LEVEL)
		{
			// 50% Jumble / 50% Error
		}
		else
		{
			// 60% Failure / 30% Error / 10% Jumble
		}
	}
}


bool RadarStabilitySystem::ShouldBlipGlitch(
	Component::Blip& blip, Component::RadarMachine& machine, float secondsSinceLastGlitch, uint32_t blipIndex, uint32_t blipCount
)
{
#ifdef DEBUG_BUILD
	if (IsKeyPressed(KEY_G)) return true;
#endif

	if (secondsSinceLastGlitch <= machine.nextGlitchSpawnSeconds) return false;
	if (blip.state != Component::Blip::Stable) return false;
	
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