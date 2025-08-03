#include "core/render_context.hpp"
#include "core/resource_store.hpp"
#include "systems/object/comms/radio_sound_system.hpp"
#include "systems/object/outside/receiver/coordinate_interpreting_system.hpp"


CoordResult CoordinateInterpretingSystem::InterpretMessageAsCoord(
    entt::registry& registry, 
    ResourceStore& resourceStore, 
    Component::Receiver& receiver,
    const std::string& message
)
{
    CoordResult result { };
	for (uint32_t i = 0u; i < message.length(); ++i)
	{
		const char character = message.at(i);

		bool isLastCharacter = i == (message.length() - 1);
		if (isLastCharacter)
		{
			switch (character) 
			{
			case 'X':
				result.axis = CoordResult::Horizontal;
				break;
			case 'Y':
				result.axis = CoordResult::Vertical;
				break;
			default:
				return result;
			}
		} else
		{
			if (!std::isdigit(character)) return result;

			int16_t digit = character - '0';
			result.coordinateLength *= 10;
			result.coordinateLength += digit;
		}
	}

	bool hasLength = result.coordinateLength > 0;

	float regionLength = 0.0f;
	switch (result.axis)
	{
	case CoordResult::Horizontal:
		regionLength = static_cast<float>(RenderContext::RADAR_SIZE.x);
		break;
	case CoordResult::Vertical:
		regionLength = static_cast<float>(RenderContext::RADAR_SIZE.y);
		break;
	default:
		break;
	}

	result.withinRegion = result.coordinateLength <= regionLength;
	result.isValid = hasLength && result.withinRegion;
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