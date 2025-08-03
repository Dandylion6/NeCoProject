#pragma once
#include "components/objects/outside/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
#include <cstdint>
#include <string>
class ResourceStore;


class AimInterpretingSystem
{
public:
	static const std::string COMMAND;

	static void HandleReceivedMessage(
		entt::registry& registry,
		ResourceStore& resourceStore,
		Component::Receiver& receiver,
		const std::string& message
	);

private:
	struct CoordResult
	{
		enum Axis: uint8_t { Invalid, Horizontal, Vertical };
		
		int16_t coordinateLength = 0;
		Axis axis = Invalid;
		bool withinRegion = false;
		bool isValid = false;
	};

	static void ConfirmAimCommand(
		entt::registry& registry,
		ResourceStore& resourceStore,
		Component::Receiver& receiver
	);

	static void HandleMessageAsCoord(
		entt::registry& registry,
		ResourceStore& resourceStore,
		Component::Receiver& receiver,
		const std::string& message
	);

	static CoordResult InterpretMessageAsCoord(const std::string& message);

	static void SetArtilleryTarget(entt::registry& registry, CoordResult result);

};