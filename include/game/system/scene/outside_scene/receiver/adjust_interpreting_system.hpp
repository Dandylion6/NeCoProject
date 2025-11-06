#pragma once
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
#include "game/system/scene/outside_scene/receiver/coordinate_interpreting_system.hpp"
#include <string>
namespace Nc { class ResourceStore; };


class AdjustInterpretingSystem
{
public:
    static const std::string COMMAND;

	static void HandleReceivedMessage(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore,
		Component::Receiver& receiver,
		const std::string& message
	);

private:
    static void ConfirmAdjustCommand(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore,
		Component::Receiver& receiver
	);

	static void AdjustArtilery(entt::registry& registry, CoordResult result);
    
};