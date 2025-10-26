#include "components/objects/interactions/toggle_component.hpp"
#include "components/objects/machine_component.hpp"
#include "core/state/anomaly_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/machine_system.hpp"


void MachineSystem::Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime)
{
	auto view = registry.view<Component::Machine>();
	for (auto [entity, machine] : view.each())
	{
		if (registry.any_of<Component::Toggle>(entity))
		{
			Component::Toggle toggle = registry.get<Component::Toggle>(entity);
			if (toggle.state != On)
			{
				anomalyState.attractionPercentage -= machine.attractionReduction * deltaTime;
				continue;
			}
		}
		// TODO: Add machine power consumption.
	}
}