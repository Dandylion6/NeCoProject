#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/scene/outside_scene/receiver/adjust_interpreting_system.hpp"
#include "game/system/scene/comms_scene/radio/radio_sound_system.hpp"
#include "game/system/scene/outside_scene/receiver/coordinate_interpreting_system.hpp"


const std::string AdjustInterpretingSystem::COMMAND = "ADJ";


void AdjustInterpretingSystem::HandleReceivedMessage(
	entt::registry& registry,
	Nc::ResourceStore& resourceStore,
	Component::Receiver& receiver,
	const std::string& message
)
{
	if (message == COMMAND)
	{
		receiver.currentContext = AdjustArtillery;
		return ConfirmAdjustCommand(registry, resourceStore, receiver);
	}

	CoordResult result = CoordinateInterpretingSystem::InterpretMessageAsCoord(registry, resourceStore, receiver, message);
	if (!result.isValid) return;

	AdjustArtilery(registry, result);

	CoordinateInterpretingSystem::ConfirmCoordinateCommand(registry, resourceStore, receiver);
}


void AdjustInterpretingSystem::ConfirmAdjustCommand(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	Component::Receiver& receiver
)
{
	const std::string RESPONSE = "assets/audio/voicelines/receiver/commands/aim_request.wav";

    Sound response = LoadSoundAlias(resourceStore.GetSound(RESPONSE));
	RadioSoundEmitterSystem::Broadcast(registry, std::move(response), Medium);

	receiver.message.clear();
}


void AdjustInterpretingSystem::AdjustArtilery(entt::registry& registry, CoordResult result)
{
	auto view = registry.view<Component::Artillery>();
	for (auto [entity, artillery] : view.each())
	{
		constexpr float DELAY = 0.75f;
		artillery.aimStartupDelay = DELAY;
		artillery.isReadyToFire = false;
		
		switch (result.axis)
		{
		case CoordResult::Invalid:
			break;
		case CoordResult::Horizontal:
			artillery.targetPosition.x = artillery.targetPosition.x + result.coordinateLength;
			break;
		case CoordResult::Vertical:
			artillery.targetPosition.y = artillery.targetPosition.y + result.coordinateLength;
			break;
		}
	}
}