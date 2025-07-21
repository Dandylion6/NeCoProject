#include "assemblers/scenes/comms_scene/morse_monitor_object.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/comms/morse_monitor.hpp"
#include "components/objects/comms/morse_transceiver_component.hpp"
#include "core/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/comms/morse_monitor_display_system.hpp"
#include "utility/interpolation.hpp"
#include <cmath>


void MorseMonitorDisplaySystem::Update(
	entt::registry& registry, MorseSettings settings, float deltaTime
)
{
	UpdatePointer(registry, settings, deltaTime);
	SetRegions(registry, settings);
}


void MorseMonitorDisplaySystem::UpdatePointer(
	entt::registry& registry, MorseSettings settings, float deltaTime
)
{
	auto view = registry.view<Tag::MorseMonitorPointer, Component::Transform>();
	for (auto [entity, transform] : view.each())
	{
		const Component::MorseTransceiver* result = GetTrasceiver(registry);
		if (result == nullptr) continue;

		const Component::MorseTransceiver& transceiver = *result;
		if (!transceiver.isInputActive)
		{
			transform.position.x = Math::SmoothApproach(
				transform.position.x, MorseMonitor::POSITION.x, deltaTime, 32.0f
			);
			continue;
		}

		float monitorScale = MorseMonitor::GAUGE_SIZE.x / settings.exitTime;
		transform.position.x = MorseMonitor::POSITION.x + monitorScale * transceiver.intervalSeconds;
	}
}


const Component::MorseTransceiver* MorseMonitorDisplaySystem::GetTrasceiver(entt::registry& registry)
{
	auto view = registry.view<const Component::MorseTransceiver>();
	for (auto [entity, transceiver] : view.each()) return &transceiver;
	return nullptr;
}


void MorseMonitorDisplaySystem::SetRegions(
	entt::registry& registry, MorseSettings settings
)
{
	float monitorScale = MorseMonitor::GAUGE_SIZE.x / settings.exitTime;
	float marginWidth = settings.errorMargin * monitorScale;

	auto view = registry.view<Component::MorseMonitorRegion, Component::Transform>();
	for (auto [entity, region, transform] : view.each())
	{
		float pulseTime = 0.0f;
		switch (region.region)
		{
		case Component::MorseMonitorRegion::Dot:
			pulseTime = settings.dotTime;
			break;
		case Component::MorseMonitorRegion::Dash:
			pulseTime = settings.dashTime;
			break;
		}

		transform.offset.x = std::round(marginWidth);
		transform.position.x = std::round(MorseMonitor::POSITION.x + pulseTime * monitorScale);
		transform.size.x = std::round(marginWidth * 2.0f);
	}
}