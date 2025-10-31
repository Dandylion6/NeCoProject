#pragma once
#include "components/objects/outside/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
class ResourceStore;


class ReceiverCodeResponseSystem
{
public:
	static void Update(
		entt::registry& registry,
		ResourceStore& resourceStore
	);

private:
	static void RespondToCharacterCode(
		entt::registry& registry,
		Component::Receiver& receiver,
		ResourceStore& resourceStore
	);

};