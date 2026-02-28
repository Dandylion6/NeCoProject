#pragma once
#include "game/component/core/transform_component.hpp"
#include "game/state/scene.hpp"
struct GameState;
struct SystemContext;


namespace Component
{
struct AudioEmitter;
}


namespace System::Audio
{

class Emitter final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context);
    static void PlayEmitter(Component::AudioEmitter& emitter);
    static void StopEmitter(Component::AudioEmitter& emitter);

private:
    // ------ Functions ------
    static void UpdateEmitter(
        Scene currentScene,
        Component::AudioEmitter& emitter,
        const Component::Transform& transform
    ) noexcept;
};

}

