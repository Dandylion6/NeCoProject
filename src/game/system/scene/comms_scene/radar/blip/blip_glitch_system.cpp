#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_glitch_system.hpp"
#include <cmath>
#include <cstdint>
#include <sstream>


void BlipGlitchSystem::Update(entt::registry& registry, float time, float deltaTime)
{
	auto view = registry.view<const Component::Transform, Component::Blip, Component::Text>();
	for (auto [entity, transform, blip, text] : view.each())
	{
		switch (blip.state) 
		{
			case Component::Blip::Stable: 
				BlipGlitchSystem:: UpdateBlipTextStable(transform, blip, text);
				break;
			case Component::Blip::CoordinateJumble:
				BlipGlitchSystem::UpdateBlipTextJumble(registry, entity, time);
				break;
			case Component::Blip::CoordinateError:
				BlipGlitchSystem::UpdateBlipTextError(registry, entity, time);
				break;
			case Component::Blip::CompleteFailure:
				BlipGlitchSystem::UpdateBlipFailure(registry, entity, time);
				break;
        }

		if (blip.state != Component::Blip::Stable) 
			blip.remainingGlitchSeconds -= deltaTime;
    }
}


void BlipGlitchSystem::JumbleBlip(
	entt::registry& registry, entt::entity entity, Component::Blip& blip, float stability
)
{
	blip.state = Component::Blip::CoordinateJumble;
	blip.remainingGlitchSeconds = GenerateGlitchDuration(stability);

	Component::BlipState::JumbledCoordindate& jumble = registry.emplace<Component::BlipState::JumbledCoordindate>(entity, stability);
	jumble = BlipGlitchSystem::GenerateRandomJumble(stability);
}


void BlipGlitchSystem::GlitchBlipText(
	entt::registry& registry, entt::entity entity, Component::Blip& blip, float stability
)
{
	blip.state = Component::Blip::CoordinateError;
	blip.remainingGlitchSeconds = GenerateGlitchDuration(stability);
	registry.emplace<Component::BlipState::CoordinateErrorData>(entity);
}


void BlipGlitchSystem::TriggerBlipFailure(
	entt::registry& registry, entt::entity entity, Component::Blip& blip, float stability
)
{
	blip.state = Component::Blip::CompleteFailure;
	blip.remainingGlitchSeconds = GenerateGlitchDuration(stability);
	registry.emplace<Component::BlipState::CompleteFailure>(entity);
}


void BlipGlitchSystem::UpdateBlipTextStable(
	const Component::Transform& transform, const Component::Blip& blip, Component::Text& text
)
{
	Nc::Vector2i pixelPosition = transform.position;
	std::ostringstream stringStream;
	stringStream << "(" << pixelPosition.x << " , " << pixelPosition.y << ")";
	text.text = stringStream.str();
}


void BlipGlitchSystem::UpdateBlipTextJumble(
	entt::registry& registry, entt::entity entity, float time
)
{
	Component::Blip& blip = registry.get<Component::Blip>(entity);

	if (blip.remainingGlitchSeconds <= 0.0f)
	{
		blip.state = Component::Blip::Stable;
		registry.remove<Component::BlipState::JumbledCoordindate>(entity);
		return;
	}

	const Component::Transform& transform = registry.get<Component::Transform>(entity);
	Component::Text& text = registry.get<Component::Text>(entity);
	Component::BlipState::JumbledCoordindate& jumble = registry.get<Component::BlipState::JumbledCoordindate>(entity);

	Nc::Vector2i pixelPosition = transform.position;
	Nc::Vector2i displayedPosition = pixelPosition;

	if (jumble.flippedAxis)
	{
		displayedPosition.x = pixelPosition.y;
		displayedPosition.y = pixelPosition.x;
	}
	
	displayedPosition.y = jumble.duplicateFirstAxis ? displayedPosition.x : displayedPosition.y;
	displayedPosition.x *= jumble.flippedSignX ? -1 : 1;

	std::ostringstream stringStream;
	stringStream << "(" << displayedPosition.x << " , " << displayedPosition.y << ")";
	text.text = stringStream.str();

	if (time - jumble.lastJumbleTime < jumble.nextJumbleSeconds) return;

	jumble = GenerateRandomJumble(jumble.stability);
	jumble.lastJumbleTime = time;

	constexpr Nc::Vector2f INTERVAL_LOW_STABILITY = Component::BlipState::JumbledCoordindate::JUMBLE_INTERVAL_LOW_RANGE;
	constexpr Nc::Vector2f INTERVAL_HIGH_STABILITY = Component::BlipState::JumbledCoordindate::JUMBLE_INTERVAL_HIGH_RANGE;
	constexpr Nc::Vector2f STABILITY_RANGE = Nc::Vector2f(Component::Radar::STABLE_LEVEL, Component::Radar::HEALTHY_LEVEL);
	constexpr Nc::Vector2f DEGREDATION_SCALE_RANGE = Nc::Vector2f(0.0f, 1.0f);

	float degradationScale = Nc::Math::Remap(STABILITY_RANGE, DEGREDATION_SCALE_RANGE, std::fmaxf(jumble.stability, Component::Radar::HEALTHY_LEVEL));
	degradationScale = Nc::Math::SineInOut(degradationScale);
	Nc::Vector2f range = Nc::Vector::Lerp(INTERVAL_HIGH_STABILITY, INTERVAL_LOW_STABILITY, degradationScale);

	float randomValue = Nc::Random::Range(0.0f, 1.0f);
	randomValue = Nc::Math::QuadIn(randomValue);
	jumble.nextJumbleSeconds = Nc::Math::Lerp(range.x, range.y, randomValue);

}


void BlipGlitchSystem::UpdateBlipTextError(
	entt::registry& registry, entt::entity entity, float time
)
{
	Component::Text& text = registry.get<Component::Text>(entity);
	Component::BlipState::CoordinateErrorData& error = registry.get<Component::BlipState::CoordinateErrorData>(entity);
	Component::Blip& blip = registry.get<Component::Blip>(entity);
	
	if (blip.remainingGlitchSeconds <= 0.0f)
	{
		blip.state = Component::Blip::Stable;
		registry.remove<Component::BlipState::CoordinateErrorData>(entity);
		return;
	}

	// Update each character.
	for (uint8_t i = 0u; i < 4u; ++i)
	{
		if (time - error.lastGlitchTimes[i] < error.nextGlitchSeconds[i]) continue;
		
		error.glitchedCharacters[i] = Nc::Random::Range(32, 126);
		error.lastGlitchTimes[i] = time;

		float randomValue = Nc::Random::Range(0.0f, 1.0f);
		Nc::Vector2f range = Component::BlipState::CoordinateErrorData::GLITCH_INTERVAL_RANGE;
		error.nextGlitchSeconds[i] = Nc::Math::Lerp(range.x, range.y, randomValue);
	}

	// Construct the display string.
	char coordDisplayX[] = { error.glitchedCharacters[0u], error.glitchedCharacters[1u], '\0' };
	char coordDisplayY[] = { error.glitchedCharacters[2u], error.glitchedCharacters[3u], '\0' };

	std::ostringstream stringStream;
	stringStream << "(" << coordDisplayX << " , " << coordDisplayY << ")";
	text.text = stringStream.str();
}


void BlipGlitchSystem::UpdateBlipFailure(
	entt::registry& registry, entt::entity entity, float time
)
{
	constexpr float RANDOM_OFFSET = Component::BlipState::CompleteFailure::OFFSET_RANGE;

	Component::Text& text = registry.get<Component::Text>(entity);
	Component::BlipState::CompleteFailure& failure = registry.get<Component::BlipState::CompleteFailure>(entity);
	const Component::Transform& transfrom = registry.get<const Component::Transform>(entity);
	Component::Blip& blip = registry.get<Component::Blip>(entity);
	
	if (blip.remainingGlitchSeconds <= 0.0f)
	{
		blip.state = Component::Blip::Stable;
		registry.remove<Component::BlipState::CompleteFailure>(entity);
		return;
	}

	text.text = "???";
	if (time - failure.lastGlitchTime < failure.nextGlitchSeconds) return;

	failure.lastGlitchTime = time;
	Nc::Vector2f range = Component::BlipState::CompleteFailure::GLITCH_INTERVAL_RANGE;
	failure.nextGlitchSeconds = Nc::Random::Range(range.x, range.y);
	
	Nc::Vector2f newGlitchOffset = Nc::Vector2f::Zero();
	newGlitchOffset.x = Nc::Random::Range(-RANDOM_OFFSET, RANDOM_OFFSET);
	newGlitchOffset.y = Nc::Random::Range(-RANDOM_OFFSET, RANDOM_OFFSET);
	failure.glitchedOffset = newGlitchOffset;
}


float BlipGlitchSystem::GenerateGlitchDuration(float stability)
{
	// The time range when radar stability is just below unstable theshold.
	constexpr Nc::Vector2f BASE_GLITCH_TIME_RANGE = Nc::Vector2f(1.6f, 6.4f);
	// The time range when radar stability is at 0.
	constexpr Nc::Vector2f MAX_GLITCH_TIME_RANGE = Nc::Vector2f(58.0f, 96.0f);
	constexpr float STABLE_FACTOR = 1.0f / Component::Radar::STABLE_LEVEL;

	float degradationScale = (Component::Radar::STABLE_LEVEL - stability) * STABLE_FACTOR;
	degradationScale = Nc::Math::SineIn(degradationScale);
	float glitchTimeMin = Nc::Math::Lerp(BASE_GLITCH_TIME_RANGE.x, MAX_GLITCH_TIME_RANGE.x, degradationScale);
	float glitchTimeMax = Nc::Math::Lerp(BASE_GLITCH_TIME_RANGE.y, MAX_GLITCH_TIME_RANGE.y, degradationScale);

	return Nc::Random::Range(glitchTimeMin, glitchTimeMax);
}


Component::BlipState::JumbledCoordindate BlipGlitchSystem::GenerateRandomJumble(float stability)
{
	Component::BlipState::JumbledCoordindate jumble = Component::BlipState::JumbledCoordindate(stability);

	jumble.duplicateFirstAxis = Nc::Random::Range(0, 10) <= 2;
	jumble.flippedAxis = Nc::Random::Range(0, 10) <= 6;
	jumble.flippedSignX = Nc::Random::Range(0, 10) <= 4;
	jumble.flippedSignY = Nc::Random::Range(0, 10) <= 4;

	return jumble;
}