#pragma once


namespace Component
{
/**
 * @brief Component that modifies audio data on this entity.
 *
 * This is applicable for <c>LoopedAudio</c>, <c>Audio</c>, and possible for more.
 * Often used for any audio that lives inside a scene.
 *
 * Usage example:
 * @code
 * const Sound& gunshotSound = resourceStore.GetSound("audio/gunshot.wav");
 * registry.emplace<Component::Audio>(entity, gunshotSound);
 * registry.emplace<Component::AudioEmitter>(entity);
 * @endcode
 */
struct AudioEmitter final
{
    // ------ Types ------

    enum State : uint8_t
    {
        Idle,
        Playing,
        Paused,
    };


    // ------ Members ------

    static constexpr float PAN_SPEED = 3.0f;
    static constexpr float VOLUME_FADE_SPEED = 6.5f;
    static constexpr float PITCH_SPEED = 2.8f;

    float stereoPan = 0.5f;
    float volume = 1.0f;
    float volumeFade = 0.0f;
    float attenuation = 0.0f;
    float pitch = 1.0f;
    State state = Idle;
    bool isSpacial = true;
    bool justPlayed = false;


    // ------ Constructors ------

    constexpr AudioEmitter() = default;
    explicit constexpr AudioEmitter(const bool isSpacial) noexcept : isSpacial(isSpacial) {}
    explicit constexpr AudioEmitter(const float volume, const float stereoPan = 0.5f, const float pitch = 1.0f) noexcept
        : stereoPan(stereoPan), volume(volume), pitch(pitch) {}
};
}
