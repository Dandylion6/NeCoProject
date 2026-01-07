#include "game/construction/scene/comms_scene/object/morse_monitor_object.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/state/settings.hpp"
#include "game/tag/scene/comms_scene/morse_monitor_tag.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/scene/comms_scene/morse_code/morse_monitor_display_system.hpp"
#include "core/math/interpolation.hpp"
#include <cmath>


void MorseMonitorDisplaySystem::Update(
	entt::registry& registry, Settings::Morse settings, float deltaTime
)
{
	UpdatePointer(registry, settings, deltaTime);
	SetRegions(registry, settings);
}


void MorseMonitorDisplaySystem::UpdatePointer(
	entt::registry& registry, Settings::Morse settings, float deltaTime
)
{
	auto view = registry.view<Tag::Morse::Monitor, Component::Transform>();
	for (auto [entity, transform] : view.each())
	{
		const Component::Morse::Transceiver* result = GetTrasceiver(registry);
		if (result == nullptr) continue;

		const Component::Morse::Transceiver& transceiver = *result;
		if (!transceiver.isInputActive)
		{
			transform.position.x = Nc::Math::SmoothApproach(
				transform.position.x, MorseMonitor::POSITION.x, deltaTime, 32.0f
			);
			continue;
		}

		float monitorScale = MorseMonitor::GAUGE_SIZE.x / settings.exitTime;
		transform.position.x = MorseMonitor::POSITION.x + monitorScale * transceiver.intervalSeconds;
	}
}


const Component::Morse::Transceiver* MorseMonitorDisplaySystem::GetTrasceiver(entt::registry& registry)
{
	auto view = registry.view<const Component::Morse::Transceiver>();
	for (auto [entity, transceiver] : view.each()) return &transceiver;
	return nullptr;
}


void MorseMonitorDisplaySystem::SetRegions(
	entt::registry& registry, Settings::Morse settings
)
{
	float monitorScale = MorseMonitor::GAUGE_SIZE.x / settings.exitTime;
	float marginWidth = settings.errorMargin * monitorScale;

	auto view = registry.view<Component::Morse::MonitorRegion, Component::Transform>();
	for (auto [entity, region, transform] : view.each())
	{
		float pulseTime = 0.0f;
		switch (region.region)
		{
		case Component::Morse::MonitorRegion::Dot:
			pulseTime = settings.dotTime;
			break;
		case Component::Morse::MonitorRegion::Dash:
			pulseTime = settings.dashTime;
			break;
		}

		transform.offset.x = std::round(marginWidth);
		transform.position.x = std::round(MorseMonitor::POSITION.x + pulseTime * monitorScale);
		transform.size.x = std::round(marginWidth * 2.0f);
	}
}