#pragma once
#include "components/objects/outside/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
#include <string>
class ResourceStore;


class RecalibrateInterpretingSystem
{
public:
	static const std::string COMMAND;

	static void HandleReceivedMessage(
		entt::registry& registry, 
		ResourceStore& resourceStore, 
		Component::Receiver& receiver, 
		const std::string& message
	);

	static void Update(entt::registry& registry, float deltaTime);

private:
	static void ConfirmRecalibrationCommand(entt::registry& registry, ResourceStore& resourceStore, Component::Receiver& receiver);

};