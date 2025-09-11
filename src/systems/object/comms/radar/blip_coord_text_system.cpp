#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"
#include "systems/object/comms/radar/blip_coord_text_system.hpp"
#include <format>


void BlipCoordTextSystem::Update(entt::registry& registry, float time, float deltaTime)
{
	auto view = registry.view<const Component::Transform, const Component::Blip, Component::Text>();
	for (auto [entity, transform, blip, text] : view.each())
	{
		switch (blip.coordState) 
		{
			case Component::Blip::CoordTextState::Stable: 
				UpdateBlipTextStable(transform, blip, text);
				break;
			case Component::Blip::CoordTextState::CoordinateJumble:
			{
				Component::Blip::JumbledCoordindate& jumble = registry.get<Component::Blip::JumbledCoordindate>(entity);
				UpdateBlipTextJumble(transform, blip, text, jumble);
				break;
			}
			case Component::Blip::CoordTextState::CompleteFailure:
				break;
        }
    }
}


Component::Blip::JumbledCoordindate BlipCoordTextSystem::GenerateRandomJumble()
{
	Component::Blip::JumbledCoordindate jumble { };

	jumble.duplicateFirstAxis = GetRandomValue(0, 10) <= 2;
	jumble.flippedAxis = GetRandomValue(0, 10) <= 6;
	jumble.flippedSignX = GetRandomValue(0, 10) <= 4;
	jumble.flippedSignY = GetRandomValue(0, 10) <= 4;

	return jumble;
}


void BlipCoordTextSystem::UpdateBlipTextStable(
	const Component::Transform& transform, const Component::Blip& blip, Component::Text& text
)
{
	Nc::Vector2i pixelPosition = transform.position.ToInt();
	text.text = std::format("({} , {})", pixelPosition.x, pixelPosition.y);
}


void BlipCoordTextSystem::UpdateBlipTextJumble(
	const Component::Transform& transform, 
	const Component::Blip& blip, 
	Component::Text& text, 
	Component::Blip::JumbledCoordindate& jumble
)
{
	Nc::Vector2i pixelPosition = transform.position.ToInt();
	Nc::Vector2i displayedPosition = pixelPosition;

	if (jumble.flippedAxis)
	{
		displayedPosition.x = pixelPosition.y;
		displayedPosition.y = pixelPosition.x;
	}

	displayedPosition.y = jumble.duplicateFirstAxis ? displayedPosition.x : displayedPosition.y;

	displayedPosition.x *= jumble.flippedSignX ? -1.0f : 1.0f;
	displayedPosition.y *= jumble.flippedSignY ? -1.0f : 1.0f;

#ifdef DEBUG_BUILD
	text.text = std::format("*({} , {})", displayedPosition.x, displayedPosition.y);
#elif 
	text.text = std::format("({} , {})", displayedPosition.x, displayedPosition.y);
#endif
}