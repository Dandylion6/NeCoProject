#pragma once
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
#include <string>
namespace Nc { class ResourceStore; };


class RecalibrateInterpretingSystem
{
public:
	static const std::string COMMAND;

	static void HandleReceivedMessage(
		entt::registry& registry, Nc::ResourceStore& resourceStore, Component::Receiver& receiver, const std::string& message
	);
	static void Update(entt::registry& registry, float deltaTime);

private:
	static void ConfirmRecalibrationCommand(entt::registry& registry, Nc::ResourceStore& resourceStore, Component::Receiver& receiver);
	static void RecalibrationCompleted(Component::Radar& radar, Component::Action::Toggle& toggle);

};