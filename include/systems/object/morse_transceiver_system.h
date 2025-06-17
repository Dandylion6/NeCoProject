#pragma once
#include "components/objects/morse_transceiver_component.h"
#include "core/scene.h"
#include "utility/morse_code.h"
#include <cstdint>
struct EcsContext;


class MorseTransceiverSystem
{
public:
	static void Update(
		EcsContext& ecsContext, 
		Scene currentScene, 
		float deltaTime
	);

private:
	static void InputChanged(Component::MorseTransceiver& transceiver);
	static void TryEndCharacter(
		EcsContext& ecsContext, Component::MorseTransceiver& transceiver
	);

	static void RecordPulse(Component::MorseTransceiver& transceiver);
	static void ClearTransceiver(Component::MorseTransceiver& transceiver);
	static MorseCode::Pulse GetPulseType(float intervalSeconds);
	
	static int8_t PulsesToChar(
		const Component::MorseTransceiver::PulseArray& pulses, 
		uint8_t pulseCount
	);

};