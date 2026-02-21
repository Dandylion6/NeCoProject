#include "game/system/scene/comms_scene/radar/radar_stability_system.hpp"

#include <cmath>

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"
#include "game/system/scene/comms_scene/radar/blip/glitch/blip_contact_failure_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/glitch/blip_distortion_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/glitch/blip_signal_noise_system.hpp"

#ifdef  DEBUG_BUILD
#include "game/component/shared/debug/runtime_readouts_component.hpp"
#endif


void System::Radar::Stability::Update(const SystemContext& context, AnomalyState& anomaly)
{
	const entt::entity entity = entt::get_single<Component::Radar>(context.registry);
	auto& radar = context.registry.get<Component::Radar>(entity);
	auto& toggle = context.registry.get<Component::Action::Toggle>(entity);

#ifdef DEBUG_BUILD
	if (IsKeyPressed(KEY_K)) radar.stability = std::fminf(radar.stability + 5.0f, 100.0f);
	if (IsKeyPressed(KEY_L)) radar.stability = std::fmaxf(radar.stability - 5.0f, 0.0f);

	const entt::entity debugEntity = entt::get_single<Component::Debug::RuntimeReadouts>(context.registry);
	auto& readouts = context.registry.get<Component::Debug::RuntimeReadouts>(debugEntity);
	readouts.radarStabilityPercentage = radar.stability;
#endif

	if (toggle.state != On) return;;

	if (!GameState::IsNight(context.game.hour)) return;

	const float degradationValue = GetDegradationValue(anomaly.attractionPercentage);
	radar.stability -= degradationValue * context.deltaTime;

	radar.breakdownCheckTimer += context.deltaTime;
	if (radar.breakdownCheckTimer >= 1.0f)
		CheckBreakdown(context.registry, radar);

	// TODO: Add effects for breakdown.
	// Turns off the radar if stability is 0.
	if (radar.stability <= 0.0f)
	{
		toggle.state = Disabled;
		return;
	}

	if (radar.nextGlitchSpawnSeconds <= 0.0f) SetRandomGlitchSpawnInterval(context.registry, radar);
	UpdateBlipStability(context, anomaly, radar);
}


void System::Radar::Stability::Restart(entt::registry& registry, const entt::entity entity)
{
	auto& radar = registry.get<Component::Radar>(entity);
	radar.stability = Component::Radar::STABLE_LEVEL;
}


void System::Radar::Stability::CheckBreakdown(entt::registry& registry, Component::Radar& radar)
{
	// The probability of spontaneous breakdown of the radar every minute.
	constexpr Nc::Vector2f BREAKDOWN_CHANCE_PER_MINUTE_RANGE = Nc::Vector2f(3.0f, 64.0f);
	constexpr Nc::Vector2f BREAKDOWN_STABILITY_RANGE = Nc::Vector2f(
		Component::Radar::STABLE_LEVEL,
		Component::Radar::UNSTABLE_LEVEL
	);
	constexpr float MINUTE_TO_SECOND = 1.0f / 60.0f;

	radar.breakdownCheckTimer = 0.0f;

	auto& randomService = registry.ctx().get<Nc::Random>();

	const float deterministicValue = randomService.RangeFloat(0.0f, 100.0f);
	const float stabilityCurve = Nc::Math::SineInOut(radar.stability * 0.01f) * 100.0f;

	const float breakdownMapped = Nc::Math::ClampedRemap(
		BREAKDOWN_STABILITY_RANGE,
		BREAKDOWN_CHANCE_PER_MINUTE_RANGE,
		stabilityCurve
	);
	const float breakdownChance = breakdownMapped * MINUTE_TO_SECOND;

	if (deterministicValue <= breakdownChance)
		radar.stability = 0.0f;
}


void System::Radar::Stability::UpdateBlipStability(
	const SystemContext& context,
	AnomalyState& anomaly,
	Component::Radar& radar
)
{
	const bool isStable = radar.stability >= Component::Radar::STABLE_LEVEL;
	const float secondsSinceLastGlitch = context.game.time - radar.lastGlitchTime;

	bool spawnNewGlitch = false;
	if (!isStable && secondsSinceLastGlitch >= radar.nextGlitchSpawnSeconds)
	{
		SetRandomGlitchSpawnInterval(context.registry, radar);
		spawnNewGlitch = true;
	}

#ifdef DEBUG_BUILD
	bool commandGlitched = IsKeyPressed(KEY_G);
#endif

	uint8_t glitchCount = 0u;
	const auto view = context.registry.view<Component::Blip>();
	for (auto [entity, blip] : view.each())
	{
		constexpr float ATTRACTION_GAIN = 1.2f;
		constexpr float STABILITY_REDUCTION = 2.0f;

#ifdef DEBUG_BUILD
		if (commandGlitched && blip.state == Component::Blip::Stable)
		{
			GlitchBlip(context, radar, blip, entity);
			commandGlitched = false;
		}
#endif

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
		if (!ShouldBlipGlitch(context.registry, view.size())) continue;

		anomaly.attractionPercentage += ATTRACTION_GAIN;
		radar.stability -= STABILITY_REDUCTION;

		radar.lastGlitchTime = context.game.time;
		spawnNewGlitch = false;
		GlitchBlip(context, radar, blip, entity);
	}

	radar.glitchCount = glitchCount;
}


bool System::Radar::Stability::ShouldBlipGlitch(entt::registry& registry, const size_t blipCount)
{
	auto& randomService = registry.ctx().get<Nc::Random>();

	const float deterministicValue = randomService.RangeFloat(0.0f, 100.0f);
	const float chance = 100.0f / static_cast<float>(blipCount);
	return deterministicValue < chance;
}


float System::Radar::Stability::GetDegradationValue(const float attractionPercentage)
{
	// The maximum duration going from 100% to 0% stability in minutes.
	constexpr float MAX_DECAY_DURATION_MINUTES = 29.0f;

	// The minimum duration going from 100% to 0% stability in minutes.
	constexpr float MIN_DECAY_DURATION_MINUTES = 5.6f;

	constexpr float MAX_DECAY_RATE = 100.0f / MAX_DECAY_DURATION_MINUTES;
	constexpr float MIN_DECAY_RATE = 100.0f / MIN_DECAY_DURATION_MINUTES;
	constexpr float MINUTE_TO_SECONDS = 1.0f / 60.0f;

	const float curveFactor = Nc::Math::SineOut(attractionPercentage * 0.01f);
	const float decayMinutes = Nc::Math::Lerp(MAX_DECAY_RATE, MIN_DECAY_RATE, curveFactor);
	return decayMinutes * MINUTE_TO_SECONDS;
}


void System::Radar::Stability::GlitchBlip(
	const SystemContext& context,
	const Component::Radar& radar,
	Component::Blip& blip,
	const entt::entity entity
)
{
	auto& randomService = context.registry.ctx().get<Nc::Random>();

	const int deterministicValue = randomService.RangeInt(0, 100);
	if (radar.stability > Component::Radar::HEALTHY_LEVEL)
	{
		Blip::Jumble::Initialize(context.registry, entity, blip, radar.stability);
		return;
	}

	if (radar.stability > Component::Radar::UNSTABLE_LEVEL)
	{
		if (deterministicValue > 30) Blip::TextError::Initialize(context.registry, entity, blip);
		else Blip::Jumble::Initialize(context.registry, entity, blip, radar.stability);
		return;
	}

	if (deterministicValue > 20) Blip::ContactFailure::Initialize(context.registry, entity, blip);
	else Blip::TextError::Initialize(context.registry, entity, blip);
}


void System::Radar::Stability::SetRandomGlitchSpawnInterval(entt::registry& registry, Component::Radar& radar)
{
	// The base interval range for new glitches to appear. Measured in minutes.
	constexpr Nc::Vector2f BASE_GLITCH_SPAWN_RANGE = Nc::Vector2f(0.8f, 1.6f);

	// How much the degradation affects the spawn interval.
	constexpr float DEGRADATION_AFFECT_SCALE = 0.7f;

	auto& randomService = registry.ctx().get<Nc::Random>();

	float minutesToNextGlitch = randomService.RangeFloat(BASE_GLITCH_SPAWN_RANGE.x, BASE_GLITCH_SPAWN_RANGE.y);
	const float degradationScale = (100.0f - radar.stability) * 0.01f;
	minutesToNextGlitch *= 1.0f - degradationScale * DEGRADATION_AFFECT_SCALE;

	radar.nextGlitchSpawnSeconds = minutesToNextGlitch * 60.0f;
}
