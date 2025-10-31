#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/shared/mechanical/machine_component.hpp"
#include "game/state/anomaly_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/shared/mechanical/machine_system.hpp"


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