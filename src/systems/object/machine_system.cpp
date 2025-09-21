#include "components/objects/machine.hpp"
#include "core/game_state.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/machine_system.hpp"


void MachineSystem::Update(entt::registry& registry, AnomalyState& anomalyState, float deltaTime)
{
	auto view = registry.view<Component::Machine>();
	for (auto [entity, machine] : view.each())
	{
		if (machine.isActive)
		{
			// TODO: Add machine power consumption.
			continue;
		}
		anomalyState.attractionPercentage -= machine.attractionReduction * deltaTime;
	}
}