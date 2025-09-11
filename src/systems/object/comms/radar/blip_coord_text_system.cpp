#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
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
				UpdateBlipTextJumble(transform, blip, text);
				break;
			case Component::Blip::CoordTextState::CompleteFailure:
				break;
        }
    }
}


void BlipCoordTextSystem::UpdateBlipTextStable(
	const Component::Transform& transform, const Component::Blip& blip, Component::Text& text
)
{
	Nc::Vector2i pixelPosition = transform.position.ToInt();
	text.text = std::format("({} , {})", pixelPosition.x, pixelPosition.y);
}


void BlipCoordTextSystem::UpdateBlipTextJumble(
	const Component::Transform& transform, const Component::Blip& blip, Component::Text& text
)
{

}