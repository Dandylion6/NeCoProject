#include "components/core/transform_component.hpp"
#include "components/objects/comms/radar.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/radar/blip_coord_text_system.hpp"


void BlipCoordTextSystem::Update(entt::registry& registry, float time, float deltaTime)
{
	auto view = registry.view<Component::Transform, Component::RadarMachine>();
	for (auto [entity, transform, machine] : view.each())
	{
		
	}
}