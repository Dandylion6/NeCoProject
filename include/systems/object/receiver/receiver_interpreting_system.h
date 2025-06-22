#pragma once
#include "components/objects/receiver_component.h"
#include "entt/entity/fwd.hpp"
#include <string>


class ReceiverInterpretingSystem
{
public:
	static void Update(entt::registry& registry);

private:

	static void TryInterpretMessage(
		const Component::Receiver& receiver, const std::string& message
	);

	static TransmissionContext TryGetContext(
		const Component::Receiver& receiver, const std::string& message
	);

};