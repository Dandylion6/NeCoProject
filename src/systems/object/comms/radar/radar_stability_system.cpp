#include "components/objects/comms/radar.hpp"
#include "components/objects/machine.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "core/state/anomaly_state.hpp"
#include "core/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/radar/blip_glitch_system.hpp"
#include "systems/object/comms/radar/radar_stability_system.hpp"
#include "utility/interpolation.hpp"
#include "utility/random.hpp"
#include "utility/vector2.hpp"
#include <cmath>
#include <cstdint>

#ifdef  DEBUG_BUILD
#include "core/game.hpp"
#include "core/context/debug_context.hpp"
#endif


void RadarStabilitySystem::Update(entt::registry& registry, GameState& gameState, float time, float deltaTime)
{
	// @brief The probablity of spotanous breakdown of the radar every minute.
	constexpr Nc::Vector2f BREAKDOWN_CHANCE_PER_MINUTE_RANGE = Nc::Vector2f(3.0f, 64.0f);
	constexpr Nc::Vector2f BREAKDOWN_STABILITY_RANGE = Nc::Vector2f(Component::Radar::STABLE_LEVEL, Component::Radar::UNSTABLE_LEVEL);
	constexpr float MINUTE_TO_SECOND = 1.0f / 60.0f;

	auto view = registry.view<Component::Machine, Component::Radar>();
	for (auto [entity, machine, radar] : view.each())
	{
#ifdef DEBUG_BUILD
		if (IsKeyPressed(KEY_K)) radar.stability = std::fminf(radar.stability + 5.0f, 100.0f);
		if (IsKeyPressed(KEY_L)) radar.stability = std::fmaxf(radar.stability - 5.0f, 0.0f);
		Game::debugContext.radarStabilityPercentage = radar.stability;
#endif

		if (!machine.isActive) continue;
		if (!GameState::IsNight(gameState.hour)) continue;

		float degradationValue = GetDegradationValue(gameState.anomalyState.attractionPercentage);
		radar.stability -= degradationValue * deltaTime;

		radar.breakdownCheckTimer += deltaTime;
		if (radar.breakdownCheckTimer >= 1.0f)
		{
			// Check for breakdown
			radar.breakdownCheckTimer = 0.0f;
			float deterministcValue = Nc::Random::Range(0.0f, 100.0f);
			float stabilityCurve = Math::SineInOut(radar.stability * 0.01f) * 100.0f;

			float breakdownMapped = Math::ClampedRemap(BREAKDOWN_STABILITY_RANGE, BREAKDOWN_CHANCE_PER_MINUTE_RANGE, stabilityCurve);
			float breakdownChance = breakdownMapped * MINUTE_TO_SECOND;

			if (deterministcValue <= breakdownChance) 
				radar.stability = 0.0f;
		}

		// TODO: Add effects for breakdown.
		// Turns off the radar if stability is 0.
		if (radar.stability <= 0.0f)
		{
			machine.isActive = false;
			continue;
		}

		bool spawnNextGlitch = radar.nextGlitchSpawnSeconds == 0.0f;
		if (spawnNextGlitch) SetRandomGlitchSpawnInterval(radar);
		UpdateBlipStability(registry, gameState.anomalyState, radar, time);
	}
}


void RadarStabilitySystem::UpdateBlipStability(
	entt::registry& registry, AnomalyState& anomalyState, Component::Radar& machine, float time
)
{
	constexpr float STABILITY_REDUCTION = 2.0f;
	constexpr float ATTRACTION_GAIN = 1.2f;

	bool isStable = machine.stability >= Component::Radar::STABLE_LEVEL;
	float secondsSinceLastGlitch = time - machine.lastGlitchTime;

	bool spawnNewGlitch = false;
	if (!isStable && secondsSinceLastGlitch >= machine.nextGlitchSpawnSeconds)
	{
		SetRandomGlitchSpawnInterval(machine);
		spawnNewGlitch = true;
	}

#ifdef DEBUG_BUILD
	bool hasCommandGliched = IsKeyPressed(KEY_G);
#endif // DEBUG_BUILD

	uint8_t glitchCount = 0u;
	auto view = registry.view<Component::Blip>();
	for (auto [entity, blip] : view.each())
	{
#ifdef DEBUG_BUILD
		if (hasCommandGliched)
		{
			if (blip.state == Component::Blip::Stable) 
			{
				hasCommandGliched = false;
				RadarStabilitySystem::GlitchBlip(registry, machine, blip, entity, time);
				break;
			}
		}
#endif // DEBUG_BUILD

		if (isStable)
		{
			// Will restore blip stability within 1 second.
			blip.remainingGlitchSeconds = std::fminf(blip.remainingGlitchSeconds, 1.0f);
			continue;
		}

		if (blip.state != Component::Blip::Stable)
		{
			++glitchCount;
			continue;
		}

		if (!spawnNewGlitch) continue;
		if (!ShouldBlipGlitch(blip, machine, secondsSinceLastGlitch, view.size())) continue;

		anomalyState.attractionPercentage += ATTRACTION_GAIN;
		machine.stability -= STABILITY_REDUCTION;

		machine.lastGlitchTime = time;
		spawnNewGlitch = false;
		RadarStabilitySystem::GlitchBlip(registry, machine, blip, entity, time);
	}

	machine.glitchCount = glitchCount;
}


bool RadarStabilitySystem::ShouldBlipGlitch(
	Component::Blip& blip, Component::Radar& machine, float secondsSinceLastGlitch, size_t blipCount
)
{
	float deterministicValue = Nc::Random::Range(0.0f, 100.0f);
	float chance = 100.0f / static_cast<float>(blipCount);
	return deterministicValue < chance;
}


float RadarStabilitySystem::GetDegradationValue(float attractionPercentage)
{
	// @brief The maximum duration going from 100% to 0% stability in minutes.
	constexpr float MAX_DECAY_DURATION_MINUTES = 29.0f;
	// @brief The minimum duration going from 100% to 0% stability in minutes.
	constexpr float MIN_DECAY_DURATION_MINUTES = 5.6f;

	constexpr float MAX_DECAY_RATE = 100.0f / MAX_DECAY_DURATION_MINUTES;
	constexpr float MIN_DECAY_RATE = 100.0f / MIN_DECAY_DURATION_MINUTES;
	constexpr float MINUTE_TO_SECONDS = 1.0f / 60.0f;

	float curveFactor = Math::SineOut(attractionPercentage * 0.01f);
	float decayMinutes = Math::Lerp(MAX_DECAY_RATE, MIN_DECAY_RATE, curveFactor);
	return decayMinutes * MINUTE_TO_SECONDS;
}


void RadarStabilitySystem::GlitchBlip(
	entt::registry& registry, Component::Radar& radar, Component::Blip& blip, const entt::entity entity, float time
)
{
	int determiniticValue = Nc::Random::Range(0, 100);
	if (radar.stability > Component::Radar::HEALTHY_LEVEL)
	{
		BlipGlitchSystem::JumbleBlip(registry, entity, blip, radar.stability);
		return;
	}

	if (radar.stability > Component::Radar::UNSTABLE_LEVEL)
	{
		if (determiniticValue > 30) BlipGlitchSystem::GlitchBlipText(registry, entity, blip, radar.stability);
		else BlipGlitchSystem::JumbleBlip(registry, entity, blip, radar.stability);
		return;
	}
	
	if (determiniticValue > 20) BlipGlitchSystem::TriggerBlipFailure(registry, entity, blip, radar.stability);
	else BlipGlitchSystem::GlitchBlipText(registry, entity, blip, radar.stability);
}


void RadarStabilitySystem::SetRandomGlitchSpawnInterval(Component::Radar& machine)
{
	// @brief The base interval range for new glitches to appear. Measured in minutes.
	constexpr Nc::Vector2f BASE_GLITCH_SPAWN_RANGE = Nc::Vector2f(0.8f, 1.6f);
	// @brief How much the degradation affects the spawn interval.
	constexpr float DEGRADATION_AFFECT_SCALE = 0.7f;

	float minutesToNextGlitch = Nc::Random::Range(BASE_GLITCH_SPAWN_RANGE.x, BASE_GLITCH_SPAWN_RANGE.y);
	float degradationScale = (100.0f - machine.stability) * 0.01f;
	minutesToNextGlitch *= 1.0f - degradationScale * DEGRADATION_AFFECT_SCALE;

	machine.nextGlitchSpawnSeconds = minutesToNextGlitch * 60.0f;
}