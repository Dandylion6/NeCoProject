#include "game/system/core/audio/looped_audio_emitter_system.hpp"

#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"


void System::Audio::LoopedEmitter::Update(const SystemContext& context)
{
    const auto view = context.registry.view<Component::Transform, Component::AudioEmitter, Component::LoopedAudio>();
    for (auto [entity, transform, emitter, audio] : view.each())
    {
        if (context.game.isPaused && transform.boundScene != NullScene)
        {
            if (CanStop(audio))
            {
                PauseMusicStream(audio.sound);
                emitter.state = Component::AudioEmitter::Paused;
            }
            continue;
        }

        UpdateMusicStream(audio.sound);

        SetMusicPan(audio.sound, emitter.stereoPan);
        SetMusicPitch(audio.sound, emitter.pitch);

        const float volume = emitter.volume * (1.0f - emitter.volumeFade);
        SetMusicVolume(audio.sound, volume * (1.0f - emitter.attenuation));

        switch (emitter.state) {
        case Component::AudioEmitter::Idle:
            if (CanStop(audio)) StopMusicStream(audio.sound);
            break;
        case Component::AudioEmitter::Playing:
            if (CanPlay(audio)) PlayMusicStream(audio.sound);
            break;
        case Component::AudioEmitter::Paused:
            if (CanPlay(audio))
            {
                ResumeMusicStream(audio.sound);
                emitter.state = Component::AudioEmitter::Playing;
            }
            break;
        }

    }
}


bool System::Audio::LoopedEmitter::CanStop(const Component::LoopedAudio& audio)
{
    if (!IsMusicValid(audio.sound)) return false;
    if (!IsMusicStreamPlaying(audio.sound)) return false;
    return true;
}


bool System::Audio::LoopedEmitter::CanPlay(const Component::LoopedAudio& audio)
{
    if (!IsMusicValid(audio.sound)) return false;
    if (IsMusicStreamPlaying(audio.sound)) return false;
    return true;
}
