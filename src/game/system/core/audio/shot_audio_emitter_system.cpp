#include "game/system/core/audio/shot_audio_emitter_system.hpp"

#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/audio_component.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"


void System::Audio::ShotEmitter::Update(const SystemContext& context)
{
    const auto view = context.registry.view<Component::Transform, Component::AudioEmitter, Component::ShotAudio>();
    for (auto [entity, transform, emitter, audio] : view.each())
    {
        if (context.game.isPaused && transform.boundScene != NullScene)
        {
            if (CanStop(audio))
            {
                PauseSound(audio.sound);
                emitter.state = Component::AudioEmitter::Playing;
            }
            continue;
        }

        if (CanStop(audio))
        {
            SetSoundPan(audio.sound, emitter.stereoPan);
            SetSoundPitch(audio.sound, emitter.pitch);

            const float volume = emitter.volume * (1.0f - emitter.volumeFade);
            SetSoundVolume(audio.sound, volume * (1.0f - emitter.attenuation));
        }

        // Audio has finished playing.
        const bool shouldBeEnd = CanPlay(audio) && !emitter.justPlayed;
        if (shouldBeEnd && emitter.state != Component::AudioEmitter::Paused)
            emitter.state = Component::AudioEmitter::Idle;

        switch (emitter.state)
        {
        case Component::AudioEmitter::Idle:
            if (CanStop(audio)) StopSound(audio.sound);
            break;
        case Component::AudioEmitter::Playing:
            if (CanPlay(audio)) PlaySound(audio.sound);
            break;
        case Component::AudioEmitter::Paused:
            if (CanPlay(audio))
            {
                ResumeSound(audio.sound);
                emitter.state = Component::AudioEmitter::Playing;
            }
            break;
        }

        emitter.justPlayed = false;
    }
}


bool System::Audio::ShotEmitter::CanStop(const Component::ShotAudio& audio)
{
    if (!IsSoundValid(audio.sound)) return false;
    if (!IsSoundPlaying(audio.sound)) return false;
    return true;
}


bool System::Audio::ShotEmitter::CanPlay(const Component::ShotAudio& audio)
{
    if (!IsSoundValid(audio.sound)) return false;
    if (IsSoundPlaying(audio.sound)) return false;
    return true;
}
