#pragma once
#include "components/objects/morse_transceiver_component.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "utility/morse_code.h"
#include <cstdint>


class MorseTransceiverSystem
{
public:
	static void Update(
		entt::registry& registry, 
		Scene currentScene, 
		float deltaTime
	);

private:
	static void InputChanged(Component::MorseTransceiver& transceiver);
	static void TryEndCharacter(
		entt::registry& registry, Component::MorseTransceiver& transceiver
	);
	static void TransmitCharacter(entt::registry& registry, char character);

	static void RecordPulse(Component::MorseTransceiver& transceiver);
	static void ClearTransceiver(Component::MorseTransceiver& transceiver);
	static MorseCode::Pulse GetPulseType(float intervalSeconds);
	
	static char PulsesToChar(
		const Component::MorseTransceiver::PulseArray& pulses, 
		uint8_t pulseCount
	);

};