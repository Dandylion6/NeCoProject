#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"
#include "systems/object/comms/radar/blip_glitch_system.hpp"
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
			{
				UpdateBlipTextJumble(registry, entity, time);
				break;
			}
			case Component::Blip::CompleteFailure:
				break;
        }

		if (blip.state != Component::Blip::Stable) 
			blip.remainingGlitchSeconds -= deltaTime;
    }
}


Component::Blip::JumbledCoordindate BlipGlitchSystem::GenerateRandomJumble()
{
	Component::Blip::JumbledCoordindate jumble { };

	jumble.duplicateFirstAxis = GetRandomValue(0, 10) <= 2;
	jumble.flippedAxis = GetRandomValue(0, 10) <= 6;
	jumble.flippedSignX = GetRandomValue(0, 10) <= 4;
	jumble.flippedSignY = GetRandomValue(0, 10) <= 4;

	return jumble;
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
		UpdateBlipTextStable(transform, blip, text);
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
	displayedPosition.x *= jumble.flippedSignX ? -1.0f : 1.0f;

#ifdef DEBUG_BUILD
	std::ostringstream stringStream;
	stringStream << "*(" << displayedPosition.x << " , " << displayedPosition.y << ")";
	text.text = stringStream.str();
#else
	std::ostringstream stringStream;
	stringStream << "(" << displayedPosition.x << " , " << displayedPosition.y << ")";
	text.text = stringStream.str();
#endif

	if (time - jumble.lastJumbleTime < jumble.nextJumbleSeconds) return;

	jumble = GenerateRandomJumble();
	jumble.lastJumbleTime = time;

	Nc::Vector2i range = Component::Blip::JumbledCoordindate::JUMBLE_INTERVAL_RANGE;
	jumble.nextJumbleSeconds = static_cast<float>(GetRandomValue(range.x, range.y)) * 0.001f;
}