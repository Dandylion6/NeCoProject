#include "game/system/scene/outside_scene/receiver/receiver_command_processor_system.hpp"

#include <string>
#include <unordered_map>

#include "raylib.h"
#include "core/runtime/entity_helpers.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"
#include "game/system/scene/outside_scene/receiver/adjust_interpreting_system.hpp"
#include "game/system/scene/outside_scene/receiver/interpret_aiming_system.hpp"
#include "game/system/scene/outside_scene/receiver/interpret_fire_system.hpp"
#include "game/system/scene/outside_scene/receiver/interpret_recalibration_system.hpp"
#include "game/utility/morse_code.hpp"

#ifdef DEBUG_BUILD
#include "game/component/shared/debug/runtime_readouts_component.hpp"
#endif


void System::Receiver::CommandProcessor::Update(const SystemContext& context)
{
	const entt::entity entity = entt::get_single<Component::Receiver>(context.registry);
	auto& receiver = context.registry.get<Component::Receiver>(entity);

#ifdef DEBUG_BUILD
	const entt::entity debugEntity = entt::get_single<Component::Debug::RuntimeReadouts>(context.registry);
	auto& readouts = context.registry.get<Component::Debug::RuntimeReadouts>(debugEntity);
	readouts.receiverMessage = receiver.message;

	if (IsKeyPressed(KEY_SLASH))
	{
		receiver.message.clear();
		readouts.receiverMessage.clear();
		return;
	}
#endif

	// Can exit early.
	if (CheckCharacterCommand(context, receiver)) return;

	receiver.message += receiver.incomingCharacter;
	ProcessMessage(context, receiver, receiver.message);
}


bool System::Receiver::CommandProcessor::CheckCharacterCommand(
	const SystemContext& context,
	Component::Receiver& receiver
)
{
	switch (receiver.incomingCharacter)
	{
	case MorseCode::NULL_CODE: return true;
	case MorseCode::BACK_CODE:
		{
			constexpr char RESPONSE[] = "assets/audio/voicelines/receiver/commands/remove_request.wav";

			if (receiver.message.empty()) return true;
			receiver.incomingCharacter = MorseCode::NULL_CODE;
			receiver.message.pop_back();

			const Sound& response = context.store.CreateSoundHandle(RESPONSE);
			Radio::Emitter::Broadcast(context.registry, response, BroadcastPriority::Low);
			return true;
		}
	case MorseCode::CANCEL_CODE:
		{
			constexpr char RESPONSE[] = "assets/audio/voicelines/receiver/commands/clear_transmission_request.wav";

			receiver.incomingCharacter = MorseCode::NULL_CODE;
			receiver.message.clear();

			const Sound& response = context.store.CreateSoundHandle(RESPONSE);
			Radio::Emitter::Broadcast(context.registry, response, BroadcastPriority::Low);
			return true;
		}
	default:
		break;
	}
	return false;
}


void System::Receiver::CommandProcessor::ProcessMessage(
	const SystemContext& context,
	Component::Receiver& receiver,
	const std::string& message
)
{
	const TransmissionContext newContext = DetermineContext(message);
	if (newContext != OnStandby) receiver.currentContext = newContext;

	switch (receiver.currentContext)
	{
	case OnStandby:
		break;
	case AimingArtillery:
		return Interpret::Aiming::HandleMessage(context, receiver, message);
	case AdjustArtillery:
		return Interpret::Adjust::HandleMessage(context, receiver, message);
	case FiringArtillery:
		return Interpret::Fire::HandleMessage(context, receiver);
	case RecalibrateRadar:
		return Interpret::Recalibration::HandleMessage(context, receiver);
	default:
		break;
	}
}


TransmissionContext System::Receiver::CommandProcessor::DetermineContext(const std::string& message)
{
	static const auto commandMap = []
	{
		std::unordered_map<std::string, TransmissionContext> map;
		map[Interpret::Aiming::COMMAND] = AimingArtillery;
		map[Interpret::Adjust::COMMAND] = AdjustArtillery;
		map[Interpret::Fire::COMMAND] = FiringArtillery;
		map[Interpret::Recalibration::COMMAND] = RecalibrateRadar;
		return map;
	}();

	if (!commandMap.contains(message)) return OnStandby;
	return commandMap.at(message);
}
