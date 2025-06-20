#include "components/objects/receiver_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/receiver_interpreting_system.h"
#include "utility/morse_code.h"
#include <iostream>
#include <string>


void ReceiverInterpretingSystem::Update(entt::registry& registry)
{
	auto view = registry.view<Component::Receiver>();
	for (auto [entity, receiver] : view.each())
	{
		if (receiver.incomingCharacter == MorseCode::NULL_CODE) continue;

		receiver.message += receiver.incomingCharacter;
		receiver.incomingCharacter = MorseCode::NULL_CODE;

		TryInterpretMessage(receiver.message);
	}
}


void ReceiverInterpretingSystem::TryInterpretMessage(std::string& message)
{
	std::cout << "Message: " << message;
}
