#include "cctype"
#include "components/objects/comms/radio_component.h"
#include "components/objects/outside/artillery_component.h"
#include "components/objects/outside/receiver_component.h"
#include "core/resource_store.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/radio_sound_system.h"
#include "systems/object/receiver/artillery_control_system.h"
#include <cstdint>
#include <string>
#include <utility>


const std::string ArtilleryControlSystem::COMMAND = "AIM";


void ArtilleryControlSystem::HandleReceivedMessage(
	entt::registry& registry,
	ResourceStore& resourceStore,
	Component::Receiver& receiver,
	const std::string& message
)
{
	if (message == COMMAND)
	{
		receiver.currentContext = AimingArtillery;
		return ConfirmAimCommand(registry, resourceStore, receiver);
	}
	TryMessageAsCoordinates(registry, resourceStore, receiver, message);
}


void ArtilleryControlSystem::ConfirmAimCommand(
	entt::registry& registry, 
	ResourceStore& resourceStore, 
	Component::Receiver& receiver
)
{
	const std::string AIM_RESPONSE = "assets/audio/voicelines/receiver/commands/aim_request.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(AIM_RESPONSE));
	RadioSoundSystem::Broadcast(registry, std::move(response), Medium);

	receiver.message.clear();
}


void ArtilleryControlSystem::TryMessageAsCoordinates(
	entt::registry& registry, 
	ResourceStore& resourceStore, 
	Component::Receiver& receiver, 
	const std::string& message
)
{
	//WIP Implementation, requires refactoring
	enum CoordinateAxis: uint8_t
	{
		Invalid, X, Y
	};

	int16_t coordinateLength = 0;
	CoordinateAxis axis = Invalid;

	for (uint32_t i = 0u; i < message.length(); ++i)
	{
		const char character = message.at(i);
		
		bool isLastCharacter = i == (message.length() - 1);
		if (isLastCharacter)
		{
			if (character == 'X') axis = X;
			else if (character == 'Y') axis = Y;

		} else
		{
			if (!std::isdigit(character)) return;

			int16_t digit = character - '0';
			coordinateLength *= 10;
			coordinateLength += digit;
		}
	}

	if (axis == Invalid || coordinateLength == 0) return;

	auto view = registry.view<Component::Artillery>();
	for (auto [entity, artillery] : view.each())
	{
		switch (axis)
		{
		case Invalid:
			break;
		case X:
			artillery.aimPosition.x = coordinateLength;
			break;
		case Y:
			artillery.aimPosition.y = coordinateLength;
			break;
		}
	}

	const std::string COORDINATE_RESPONSE = "assets/audio/voicelines/receiver/commands/coordinate_received.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(COORDINATE_RESPONSE));
	RadioSoundSystem::Broadcast(registry, std::move(response), Medium);

	receiver.message.clear();
};