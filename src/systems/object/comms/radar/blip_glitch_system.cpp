#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/blip_glitch_system.hpp"
#include "utility/random.hpp"
#include "utility/vector2.hpp"
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
				UpdateBlipTextStable(transform, blip, text);
				break;
			case Component::Blip::CoordinateJumble:
				UpdateBlipTextJumble(registry, entity, time);
				break;
			case Component::Blip::CoordinateError:
				UpdateBlipTextError(registry, entity, time);
				break;
			case Component::Blip::CompleteFailure:
				UpdateBlipFailure(registry, entity, time);
				break;
        }

		if (blip.state != Component::Blip::Stable) 
			blip.remainingGlitchSeconds -= deltaTime;
    }
}


void BlipGlitchSystem::JumbleBlip(
	entt::registry& registry, entt::entity entity, Component::Blip& blip
)
{
	blip.state = Component::Blip::CoordinateJumble;
	Component::Blip::JumbledCoordindate& jumble = registry.emplace<Component::Blip::JumbledCoordindate>(entity);
	jumble = BlipGlitchSystem::GenerateRandomJumble();

	Nc::Vector2f glitchTimeRange = Component::Blip::BASE_GLITCH_TIME_RANGE;
	blip.remainingGlitchSeconds = Nc::Random::Range(glitchTimeRange.x, glitchTimeRange.y);
}


void BlipGlitchSystem::GlitchBlipText(
	entt::registry& registry, entt::entity entity, Component::Blip& blip
)
{
	blip.state = Component::Blip::CompleteFailure;
	registry.emplace<Component::Blip::CoordinateErrorData>(entity);

	Nc::Vector2f glitchTimeRange = Component::Blip::BASE_GLITCH_TIME_RANGE;
	blip.remainingGlitchSeconds = Nc::Random::Range(glitchTimeRange.x, glitchTimeRange.y);
}


void BlipGlitchSystem::TriggerBlipFailure(
	entt::registry& registry, entt::entity entity, Component::Blip& blip
)
{
	blip.state = Component::Blip::CompleteFailure;
	registry.emplace<Component::Blip::CoordinateErrorData>(entity);

	Nc::Vector2f glitchTimeRange = Component::Blip::BASE_GLITCH_TIME_RANGE;
	blip.remainingGlitchSeconds = Nc::Random::Range(glitchTimeRange.x, glitchTimeRange.y);
}


void BlipGlitchSystem::UpdateBlipTextStable(
	const Component::Transform& transform, const Component::Blip& blip, Component::Text& text
)
{
	Nc::Vector2i pixelPosition = transform.position.ToInt();
	std::ostringstream stringStream;
	stringStream << "(" << pixelPosition.x << " , " << pixelPosition.y << ")";
	text.text = stringStream.str();
}


void BlipGlitchSystem::UpdateBlipTextJumble(
	entt::registry& registry, entt::entity entity, float time
)
{
	const Component::Transform& transform = registry.get<Component::Transform>(entity);
	Component::Blip& blip = registry.get<Component::Blip>(entity);
	Component::Text& text = registry.get<Component::Text>(entity);
	Component::Blip::JumbledCoordindate& jumble = registry.get<Component::Blip::JumbledCoordindate>(entity);

	if (blip.remainingGlitchSeconds <= 0.0f)
	{
		blip.state = Component::Blip::Stable;
		registry.remove<Component::Blip::JumbledCoordindate>(entity);
		return;
	}

	Nc::Vector2i pixelPosition = transform.position.ToInt();
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

	jumble = GenerateRandomJumble();
	jumble.lastJumbleTime = time;

	Nc::Vector2f range = Component::Blip::JumbledCoordindate::JUMBLE_INTERVAL_RANGE;
	jumble.nextJumbleSeconds = Nc::Random::Range(range.x, range.y);
}


void BlipGlitchSystem::UpdateBlipTextError(
	entt::registry& registry, entt::entity entity, float time
)
{
	Component::Text& text = registry.get<Component::Text>(entity);
	Component::Blip::CoordinateErrorData& error = registry.get<Component::Blip::CoordinateErrorData>(entity);
	Component::Blip& blip = registry.get<Component::Blip>(entity);
	
	if (blip.remainingGlitchSeconds <= 0.0f)
	{
		blip.state = Component::Blip::Stable;
		registry.remove<Component::Blip::CoordinateErrorData>(entity);
		return;
	}

	// Update each character.
	for (uint8_t i = 0u; i < 4u; ++i)
	{
		if (time - error.lastGlitchTimes[i] < error.nextGlitchSeconds[i]) continue;
		
		error.glitchedCharacters[i] = Nc::Random::Range(32, 126);
		error.lastGlitchTimes[i] = time;
		Nc::Vector2f range = Component::Blip::CoordinateErrorData::GLITCH_INTERVAL_RANGE;
		error.nextGlitchSeconds[i] = Nc::Random::Range(range.x, range.y);
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
	constexpr int RANDOM_OFFSET = Component::Blip::CompleteFailureData::OFFSET_RANGE;

	Component::Text& text = registry.get<Component::Text>(entity);
	Component::Blip::CompleteFailureData& failure = registry.get<Component::Blip::CompleteFailureData>(entity);
	const Component::Transform& transfrom = registry.get<const Component::Transform>(entity);
	Component::Blip& blip = registry.get<Component::Blip>(entity);
	
	if (blip.remainingGlitchSeconds <= 0.0f)
	{
		blip.state = Component::Blip::Stable;
		registry.remove<Component::Blip::CompleteFailureData>(entity);
		return;
	}

	text.text = "???";
	if (time - failure.lastGlitchTime < failure.nextGlitchSeconds) return;

	failure.lastGlitchTime = time;
	Nc::Vector2f range = Component::Blip::CompleteFailureData::GLITCH_INTERVAL_RANGE;
	failure.nextGlitchSeconds = Nc::Random::Range(range.x, range.y);
	
	Nc::Vector2f newGlitchOffset = Nc::Vector2f::Zero();
	newGlitchOffset.x = Nc::Random::Range(-RANDOM_OFFSET, RANDOM_OFFSET);
	newGlitchOffset.y = Nc::Random::Range(-RANDOM_OFFSET, RANDOM_OFFSET);
	failure.glitchedOffset = newGlitchOffset;
}


Component::Blip::JumbledCoordindate BlipGlitchSystem::GenerateRandomJumble()
{
	Component::Blip::JumbledCoordindate jumble { };

	jumble.duplicateFirstAxis = Nc::Random::Range(0, 10) <= 2;
	jumble.flippedAxis = Nc::Random::Range(0, 10) <= 6;
	jumble.flippedSignX = Nc::Random::Range(0, 10) <= 4;
	jumble.flippedSignY = Nc::Random::Range(0, 10) <= 4;

	return jumble;
}