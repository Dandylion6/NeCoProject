#include "cctype"
#include "components/objects/comms/radio_component.hpp"
#include "components/objects/outside/receiver_component.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "systems/object/comms/radio_sound_system.hpp"
#include "systems/object/outside/receiver/coordinate_interpreting_system.hpp"
#include <cstdint>
#include <string>
#include <utility>


CoordResult CoordinateInterpretingSystem::InterpretMessageAsCoord(
    entt::registry& registry, 
    ResourceStore& resourceStore, 
    Component::Receiver& receiver,
    const std::string& message
)
{
    CoordResult result { };

	if (message.empty()) return result;

	const char suffix = message.at(message.length() - 1u);
	switch (suffix) {
		case 'X':
			result.axis = CoordResult::Horizontal;
			break;
		case 'Y':
			result.axis = CoordResult::Vertical;
			break;
		default:
			return result;
	}

	uint32_t startingIndex = 0u;
	int16_t sign = 1;
	const char prefix = message.at(0u);
	if (prefix == 'N')
	{
		sign = -1;
		++startingIndex;
	}

	// Check if there are any digits, if not don't assume it is a coordinate command.
	bool hasDigit = false;
	for (uint32_t i = startingIndex; i < message.length() - 1u; ++i)
	{
		const char character = message.at(i);

		if (!std::isdigit(character)) return result;
		hasDigit = true;

		int16_t digit = character - '0';
		result.coordinateLength *= 10;
		result.coordinateLength += digit * sign;
	}

	result.isValid = hasDigit;
	return result;
}


void CoordinateInterpretingSystem::ConfirmCoordinateCommand(
	entt::registry& registry,
	ResourceStore& resourceStore,
	Component::Receiver& receiver
)
{
    const std::string COORDINATE_RESPONSE = "assets/audio/voicelines/receiver/commands/coordinate_received.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(COORDINATE_RESPONSE));
	RadioSoundSystem::Broadcast(registry, std::move(response), Medium);

	receiver.message.clear();
}