#include "game/system/shared/mechanical/machine_power_system.hpp"

#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/shared/mechanical/machine_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"


void System::Machine::PowerUsage::Update(const SystemContext& context, AnomalyState& anomaly)
{
	const auto view = context.registry.view<Component::Machine>();
	for (auto [entity, machine] : view.each())
	{
		if (!IsMachineActive(context, entity)) continue;
		anomaly.attractionPercentage -= machine.attractionReduction * context.deltaTime;
		// TODO: Add machine power consumption.
	}
}


bool System::Machine::PowerUsage::IsMachineActive(const SystemContext& context, const entt::entity machine) noexcept
{
	if (context.game.isPaused) return false;
	if (!GameState::IsNight(context.game.hour)) return false;

	if (context.registry.all_of<Component::Action::Toggle>(machine))
	{
		const auto toggle = context.registry.get<Component::Action::Toggle>(machine);
		if (toggle.state != On) return false;
	}
	return true;
}
