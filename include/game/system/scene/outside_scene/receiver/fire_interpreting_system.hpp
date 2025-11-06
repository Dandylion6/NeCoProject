#pragma once
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
#include <string>
namespace Nc { class ResourceStore; };


class FireInterpretingSystem
{
public:
	static const std::string COMMAND;

	static void HandleReceivedMessage(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore,
		Component::Receiver& receiver,
		const std::string& message
	);

	static void Update(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore,
		float deltaTime
	);

};