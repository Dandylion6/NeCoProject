#include "components/core/transform_component.h"
#include "components/objects/comms/morse_transceiver_component.h"
#include "components/objects/outside/receiver_component.h"
#ifdef DEBUG_BUILD
#include "core/debug_context.h"
#include "core/game.h"
#endif // DEBUG_BUILD
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/morse_transceiver_system.h"
#include "utility/morse_code.h"
#include <cmath>
#include <cstdint>
#include <string>
#include "core/game_state.h"
#include "utility/vector2.h"


void MorseTransceiverSystem::Update(
	entt::registry& registry, 
	Scene currentScene, 
	MorseSettings settings,
	float deltaTime
)
{
	auto view = registry.view<Component::Transform, Component::MorseTransceiver>();
	for (auto [entity, transform, transceiver] : view.each())
	{
		if (currentScene != transform.boundScene) continue;

		bool inputKeyPressed = IsKeyDown(Component::MorseTransceiver::INPUT_KEY);
		bool inputStateChanged = inputKeyPressed != transceiver.isInputActive;

		if (inputStateChanged) InputChanged(transceiver, settings);
		else if (!inputKeyPressed) TryEndCharacter(registry, transceiver, settings);

		transceiver.isInputActive = inputKeyPressed;
		
		float increasedInverval = transceiver.intervalSeconds + deltaTime;
		float maxInterval = settings.dashTime * 3.0f;
		transceiver.intervalSeconds = std::fminf(increasedInverval, maxInterval);

#ifdef DEBUG_BUILD
		if (!transceiver.isInputActive)
		{
			Game::debugContext.pulse = MorseCode::Invalid;
			continue;
		}
		Game::debugContext.pulse = GetPulseType(transceiver.intervalSeconds, settings);
#endif // DEBUG_BUILD
	}
}


void MorseTransceiverSystem::InputChanged(
	Component::MorseTransceiver& transceiver,
	MorseSettings settings
)
{
	bool inputJustStarted = IsKeyPressed(Component::MorseTransceiver::INPUT_KEY);
	if (inputJustStarted)
	{
		transceiver.intervalSeconds = 0.0f;
	} else RecordPulse(transceiver, settings); // Input just stopped
}


void MorseTransceiverSystem::TryEndCharacter(
	entt::registry& registry, 
	Component::MorseTransceiver& transceiver, 
	MorseSettings settings
)
{
	float longestTime = settings.dashTime + settings.errorMargin;
	bool shouldEndCharacter = transceiver.intervalSeconds > longestTime;
	
	if (!shouldEndCharacter) return;
	if (transceiver.pulseCount == 0u) return;

	int8_t character = PulsesToChar(transceiver.pulses, transceiver.pulseCount);

	TransmitCharacter(registry, character);
	ClearTransceiver(transceiver);
}


void MorseTransceiverSystem::TransmitCharacter(
	entt::registry& registry, char character
)
{
	auto receiverView = registry.view<Component::Receiver>();
	for (auto [entity, receiver] : receiverView.each())
	{
		receiver.incomingCharacter = character;
	}
}


void MorseTransceiverSystem::RecordPulse(
	Component::MorseTransceiver& transceiver, MorseSettings settings
)
{
	if (transceiver.pulseCount >= Component::MorseTransceiver::MAX_PULSES) return;
	
	transceiver.pulses[transceiver.pulseCount] = GetPulseType(transceiver.intervalSeconds, settings);
	++transceiver.pulseCount;
	transceiver.intervalSeconds = 0.0f;
}


void MorseTransceiverSystem::ClearTransceiver(Component::MorseTransceiver& transceiver)
{
	transceiver.intervalSeconds = 0.0f;
	transceiver.pulses.fill(MorseCode::Invalid);
	transceiver.pulseCount = 0u;
}


MorseCode::Pulse MorseTransceiverSystem::GetPulseType(
	float intervalSeconds, MorseSettings settings
)
{
	Nc::Vector2f margins = Nc::Vector2f::Zero();
	margins.x = settings.dotTime - settings.errorMargin;
	margins.y = settings.dotTime + settings.errorMargin;

	if (intervalSeconds >= margins.x && intervalSeconds <= margins.y)
		return MorseCode::Short;
	
	margins.x = settings.dashTime - settings.errorMargin;
	margins.y = settings.dashTime + settings.errorMargin;

	if (intervalSeconds >= margins.x && intervalSeconds <= margins.y)
		return MorseCode::Long;

	return MorseCode::Invalid;
}


char MorseTransceiverSystem::PulsesToChar(
	const Component::MorseTransceiver::PulseArray& pulses,
	uint8_t pulseCount
)
{
	std::string codeString { };
	for (uint8_t i = 0u; i < pulseCount; ++i)
	{
		MorseCode::Pulse pulse = pulses.at(i);
		switch (pulse)
		{
		case MorseCode::Invalid: return MorseCode::NULL_CODE;
		case MorseCode::Short: codeString += '.'; break;
		case MorseCode::Long: codeString += '-'; break;
		default: return MorseCode::NULL_CODE;
		}
	}
	return MorseCode::GetChar(codeString);
}
