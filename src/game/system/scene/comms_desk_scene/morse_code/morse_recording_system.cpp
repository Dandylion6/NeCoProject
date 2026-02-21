#include "game/system/scene/comms_desk_scene/morse_code/morse_recording_system.hpp"

#include <cstdint>

#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/settings.hpp"
#include "game/utility/morse_code.hpp"


void System::Morse::Recording::Update(const SystemContext& context, const Settings::Morse settings) noexcept
{
    const entt::entity entity = entt::get_single<Component::Morse::Transceiver>(context.registry);
    auto& transceiver = context.registry.get<Component::Morse::Transceiver>(entity);

    transceiver.intervalSeconds += context.deltaTime;
    const float exitSeconds = MorseCode::ExitTime(settings.dotSeconds);

    if (!transceiver.isPushed)
    {
        if (transceiver.inputJustChanged)
            TryRecordPulse(transceiver, settings.dotSeconds);

        if (transceiver.intervalSeconds >= exitSeconds)
        {
            TryTransmitCharacter(context, transceiver.decodingIndex);
            transceiver.decodingIndex = 1;
        }
    }

    transceiver.intervalSeconds = std::min(transceiver.intervalSeconds, exitSeconds);

    if (transceiver.inputJustChanged)
        transceiver.intervalSeconds = 0.0f;
}


void System::Morse::Recording::TryRecordPulse(Component::Morse::Transceiver& transceiver, const float dotSeconds) noexcept
{
    switch (GetType(transceiver.intervalSeconds, dotSeconds))
    {
    case Invalid:
        transceiver.decodingIndex = 1;
        break;
    case Short: transceiver.decodingIndex *= 2;
        break;
    case Long: transceiver.decodingIndex = transceiver.decodingIndex * 2 + 1;
        break;
    }
}


void System::Morse::Recording::TryTransmitCharacter(const SystemContext& context, const uint16_t decodingIndex) noexcept
{
    const char character = MorseCode::TABLE[decodingIndex];
    if (character == MorseCode::NULL_CODE) return;

    const entt::entity entity = entt::get_single<Component::Receiver>(context.registry);
    auto& receiver = context.registry.get<Component::Receiver>(entity);
    receiver.incomingCharacter = character;
}


System::Morse::Recording::Type System::Morse::Recording::GetType(const float intervalSeconds, const float dotSeconds) noexcept
{
    const float margin = MorseCode::ErrorMargin(dotSeconds);
    const float toDotDifference = std::abs(intervalSeconds - dotSeconds);

    if (toDotDifference <= margin) return Short;

    const float dashSeconds = MorseCode::DashTime(dotSeconds);
    const float toDashDifference = std::abs(intervalSeconds - dashSeconds);

    if (toDashDifference <= margin) return Long;

    return Invalid;
}
