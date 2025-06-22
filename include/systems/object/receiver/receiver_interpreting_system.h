#pragma once
#include "components/objects/receiver_component.h"
#include "entt/entity/fwd.hpp"
#include <string>
class ResourceStore;


class ReceiverInterpretingSystem
{
public:
	static void Update(
		entt::registry& registry,
		ResourceStore& resourceStore
	);

private:

	static void TryInterpretMessage(
		entt::registry& registry,
		ResourceStore& resourceStore,
		const Component::Receiver& receiver, 
		const std::string& message
	);

	static TransmissionContext TryGetContext(
		const Component::Receiver& receiver, const std::string& message
	);

};