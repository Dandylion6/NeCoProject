#include "game/system/scene/outside_scene/receiver/adjust_interpreting_system.hpp"

#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"
#include "game/system/scene/outside_scene/receiver/interpret_coordinate_system.hpp"


void System::Receiver::Interpret::Adjust::HandleMessage(
	const SystemContext& context,
	Component::Receiver& receiver,
	const std::string& message
)
{
	if (message == COMMAND)
	{
		receiver.currentContext = TransmissionContext::AdjustArtillery;
		return ConfirmCommand(context, receiver);
	}

	const CoordResult result = Coordinate::MessageAsCoord(message);
	if (!result.isValid) return;

	AdjustArtillery(context.registry, result);

	Coordinate::ConfirmCommand(context, receiver);
}


void System::Receiver::Interpret::Adjust::ConfirmCommand(const SystemContext& context, Component::Receiver& receiver)
{
	constexpr char RESPONSE[] = "assets/audio/voicelines/receiver/commands/aim_request.wav";

	const Sound& response = context.store.CreateSoundHandle(RESPONSE);
	Radio::Emitter::Broadcast(context.registry, std::move(response), BroadcastPriority::Medium);

	receiver.message.clear();
}


void System::Receiver::Interpret::Adjust::AdjustArtillery(entt::registry& registry, const CoordResult result)
{
	constexpr float DELAY = 0.75f;

	const entt::entity entity = entt::get_single<Component::Artillery>(registry);
	auto& artillery = registry.get<Component::Artillery>(entity);

	artillery.aimStartupSeconds = DELAY;
	artillery.isReadyToFire = false;

	switch (result.axis)
	{
	case CoordResult::Invalid: break;
	case CoordResult::Horizontal:
		artillery.targetPosition.x = artillery.targetPosition.x + result.coordinateLength;
		break;
	case CoordResult::Vertical:
		artillery.targetPosition.y = artillery.targetPosition.y + result.coordinateLength;
		break;
	}
}
