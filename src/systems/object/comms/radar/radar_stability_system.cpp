#include "components/objects/comms/radar.hpp"
#include "components/objects/machine.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/radar/blip_glitch_system.hpp"
#include "systems/object/comms/radar/radar_stability_system.hpp"
#include "utility/random.hpp"
#include "utility/vector2.hpp"
#include <cmath>
#include <cstdint>

#ifdef  DEBUG_BUILD
#include "core/game.hpp"
#include "core/debug_context.hpp"
#endif


void RadarStabilitySystem::Update(entt::registry& registry, AnomalyState& anomalyState, float time, float deltaTime)
{
	auto view = registry.view<Component::Machine, Component::Radar>();
	for (auto [entity, machine, radar] : view.each())
	{
#ifdef DEBUG_BUILD
		if (IsKeyPressed(KEY_K)) radar.stability = std::fminf(radar.stability + 5.0f, 100.0f);
		if (IsKeyPressed(KEY_L)) radar.stability = std::fmaxf(radar.stability - 5.0f, 0.0f);
		Game::debugContext.radarStabilityPercentage = radar.stability;
#endif

		if (!machine.isActive) continue;

		if (anomalyState.attractionPercentage >= Component::Radar::DEGRADATION_THRESHOLD)
		{
			float attractionToLength = std::roundf(anomalyState.attractionPercentage  / Component::Radar::CURVE_CACHE_SIZE);
			uint8_t cacheIndex = static_cast<uint8_t>(attractionToLength);
			radar.stability -= radar.degredationCurveCache[cacheIndex] * deltaTime;

			if (radar.nextGlitchSpawnSeconds == 0.0f)
				SetRandomGlitchSpawnInterval(radar);
		}

		// Turns off the radar if stability is 0.
		if (radar.stability <= 0.0f) machine.isActive = false;

		UpdateBlipStability(registry, radar, time);
	}
}


void RadarStabilitySystem::UpdateBlipStability(entt::registry& registry, Component::Radar& machine, float time)
{
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


void RadarStabilitySystem::GenerateCurveCache(Component::Radar& radar)
{
	constexpr float DEGRADATION_FACTOR = 7.6f;
	constexpr float DEGRADATION_CURVE = 3.1f;
	constexpr float MINUTE_TO_SECOND_FACTOR = 1.0f / 60.0f;

	float percentagePerIndex = 100.0f / static_cast<float>(Component::Radar::CURVE_CACHE_SIZE);
	for (int i = 0; i < Component::Radar::CURVE_CACHE_SIZE; ++i)
	{
		float percentage = static_cast<float>(i) * percentagePerIndex;
		float adjustedPercentage = percentage - Component::Radar::DEGRADATION_THRESHOLD;
		float curveValue = DEGRADATION_FACTOR * std::powf(DEGRADATION_CURVE, adjustedPercentage * Component::Radar::PERCENTAGE_FACTOR) - 1.0f;
		radar.degredationCurveCache[i] = curveValue * MINUTE_TO_SECOND_FACTOR;
	}
}