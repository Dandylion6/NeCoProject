#pragma once
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


class ReceiverCodeResponseSystem
{
public:
	static void Update(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore
	);

private:
	static void RespondToCharacterCode(
		entt::registry& registry,
		Component::Receiver& receiver,
		Nc::ResourceStore& resourceStore
	);

};