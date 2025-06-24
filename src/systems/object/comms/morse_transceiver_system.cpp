#include "components/core/transform_component.h"
#include "components/objects/comms/morse_transceiver_component.h"
#include "components/objects/outside/receiver_component.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/morse_transceiver_system.h"
#include "utility/morse_code.h"
#include <cmath>
#include <cstdint>
#include <string>


void MorseTransceiverSystem::Update(
	entt::registry& registry, Scene currentScene, float deltaTime
)
{
	auto view = registry.view<Component::Transform, Component::MorseTransceiver>();
	for (auto [entity, transform, transceiver] : view.each())
	{
		if (currentScene != transform.boundScene) continue;

		bool inputKeyPressed = IsKeyDown(Component::MorseTransceiver::INPUT_KEY);
		bool inputStateChanged = inputKeyPressed != transceiver.isInputActive;

		if (inputStateChanged) InputChanged(transceiver);
		else if (!inputKeyPressed) TryEndCharacter(registry, transceiver);

		transceiver.isInputActive = inputKeyPressed;
		float increasedInverval = transceiver.intervalSeconds + deltaTime;
		constexpr float MAX_INTERVAL = MorseCode::LONG_DURATION * 2.0f;
		transceiver.intervalSeconds = std::fminf(increasedInverval, MAX_INTERVAL);
	}
}


void MorseTransceiverSystem::InputChanged(Component::MorseTransceiver& transceiver)
{
	bool inputJustStarted = IsKeyPressed(Component::MorseTransceiver::INPUT_KEY);
	if (inputJustStarted)
	{
		transceiver.intervalSeconds = 0.0f;
	} else RecordPulse(transceiver); // Input just stopped
}


void MorseTransceiverSystem::TryEndCharacter(
	entt::registry& registry, Component::MorseTransceiver& transceiver
)
{
	bool shouldEndCharacter = transceiver.intervalSeconds > MorseCode::LONG_DURATION;
	
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


void MorseTransceiverSystem::RecordPulse(Component::MorseTransceiver& transceiver)
{
	if (transceiver.pulseCount >= Component::MorseTransceiver::MAX_PULSES) return;
	
	transceiver.pulses[transceiver.pulseCount] = GetPulseType(transceiver.intervalSeconds);
	++transceiver.pulseCount;
	transceiver.intervalSeconds = 0.0f;
}


void MorseTransceiverSystem::ClearTransceiver(Component::MorseTransceiver& transceiver)
{
	transceiver.intervalSeconds = 0.0f;
	transceiver.pulses.fill(MorseCode::Invalid);
	transceiver.pulseCount = 0u;
}


MorseCode::Pulse MorseTransceiverSystem::GetPulseType(float intervalSeconds)
{

	float dotDifference = std::fabsf(intervalSeconds - MorseCode::SHORT_DURATION);
	float dashDifference = std::fabsf(intervalSeconds - MorseCode::LONG_DURATION);

	bool isShortPulse = dotDifference < dashDifference;
	return isShortPulse ? MorseCode::Short : MorseCode::Long;
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
