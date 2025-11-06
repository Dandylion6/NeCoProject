#pragma once
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
#include <string>
namespace Nc { class ResourceStore; };


class ReceiverInterpretingSystem
{
public:
	static void Update(
		entt::registry& registry, Nc::ResourceStore& resourceStore
	);

private:

	static void TryInterpretMessage(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore,
		Component::Receiver& receiver, 
		const std::string& message
	);

	static TransmissionContext TryGetContext(
		const Component::Receiver& receiver, const std::string& message
	);

};