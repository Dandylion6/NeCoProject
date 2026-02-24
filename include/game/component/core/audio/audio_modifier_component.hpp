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
 * registry.emplace<Component::AudioModifier>(entity);
 * @endcode
 */
struct AudioModifier final
{
    // ------ Members ------

    static constexpr float PAN_SPEED = 3.0f;
    static constexpr float VOLUME_SPEED = 2.5f;
    static constexpr float PITCH_SPEED = 2.8f;

    float stereoPan = 0.5f;
    float volume = 1.0f;
    float pitch = 1.0f;


    // ------ Constructors ------

    constexpr AudioModifier() = default;
    constexpr AudioModifier(const float stereoPan, const float volume) noexcept
        : stereoPan(stereoPan), volume(volume) {}
};
}
