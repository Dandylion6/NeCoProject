#include "game/system/scene/comms_scene/morse_code/morse_monitor_display_system.hpp"

#include <cmath>

#include "core/math/interpolation.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
#include "game/construction/scene/comms_scene/object/morse_monitor_object.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/settings.hpp"
#include "game/tag/scene/comms_scene/morse_monitor_tag.hpp"


void System::Morse::MonitorDisplay::Update(const SystemContext& context, const Settings::Morse settings)
{
	const float monitorScale = Object::MorseMonitor::GAUGE_SIZE.x / MorseCode::ExitTime(settings.dotTime);
	UpdatePointer(context, monitorScale);
	SetRegions(context.registry, settings, monitorScale);
}


void System::Morse::MonitorDisplay::UpdatePointer(const SystemContext& context, const float monitorScale)
{
	constexpr float END_POSITION = Object::MorseMonitor::POSITION.x;

	const entt::entity entity = entt::get_single<Tag::Morse::Monitor>(context.registry);
	auto& transform = context.registry.get<Component::Transform>(entity);

	const Component::Morse::Transceiver* result = GetTransceiver(context.registry);
	if (result == nullptr) return;

	const Component::Morse::Transceiver& transceiver = *result;
	if (!transceiver.isInputActive)
	{
		constexpr float SMOOTH_SPEED = 32.0f;

		transform.position.x = Nc::Math::SmoothApproach(
			transform.position.x,
			END_POSITION,
			context.deltaTime,
			SMOOTH_SPEED
		);
		return;
	}
	transform.position.x = END_POSITION + monitorScale * transceiver.intervalSeconds;
}


const Component::Morse::Transceiver* System::Morse::MonitorDisplay::GetTransceiver(entt::registry& registry)
{
	const entt::entity entity = entt::get_single<Component::Morse::Transceiver>(registry);
	if (entity == entt::null) return nullptr;
	return &registry.get<Component::Morse::Transceiver>(entity);
}


void System::Morse::MonitorDisplay::SetRegions(
	entt::registry& registry,
	const Settings::Morse settings,
	const float monitorScale
)
{
	const float marginWidth = MorseCode::ErrorMargin(settings.dotTime) * monitorScale;

	const auto view = registry.view<Component::Morse::MonitorRegion, Component::Transform>();
	for (auto [entity, region, transform] : view.each())
	{
		float pulseTime = 0.0f;
		switch (region.region)
		{
		case Component::Morse::MonitorRegion::Dot:
			pulseTime = settings.dotTime;
			break;
		case Component::Morse::MonitorRegion::Dash:
			pulseTime = MorseCode::DashTime(settings.dotTime);
			break;
		}

		transform.offset.x = std::roundf(marginWidth);
		transform.position.x = std::roundf(Object::MorseMonitor::POSITION.x + pulseTime * monitorScale);
		transform.size.x = std::roundf(marginWidth * 2.0f);
	}
}
