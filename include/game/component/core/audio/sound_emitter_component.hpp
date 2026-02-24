#pragma once
#include "raylib.h"


namespace Component
{
/**
 * @brief Component that allows an entity to emit a one-shot sound.
 *
 * This component represents a sound that can be played once or on demand,
 * such as gunshots, footsteps, or UI clicks.
 *
 * The sound data is stored directly as a <c>Sound</c> object from raylib, meaning
 * it should be preloaded or retrieved from a resource store to avoid
 * repeated loading. The <c>volume</c> value scales playback volume (1.0f = full volume).
 *
 * Usage example:
 * @code
 * const Sound& gunshotSound = resourceStore.GetSound("audio/gunshot.wav");
 * registry.emplace<Component::Audio>(entity, gunshotSound);
 * @endcode
 */
struct Audio final
{
	// ------ Members ------

	Sound sound{ };
	float volume = 1.0f;

	
	// ------ Constructors ------

	constexpr Audio() noexcept = default;
	explicit constexpr Audio(const Sound& sound) noexcept : sound(sound) { };
};


/**
 * @brief Component that allows an entity to continuously emit looping sound or music.
 *
 * Designed for background audio or continuous sound effects such as ambience,
 * engine hums, or music tracks. The underlying <c>Music</c> object in raylib handles
 * streaming automatically, making it suitable for large or long audio files.
 *
 * Like <c>SoundEmitter</c>, the sound should be preloaded or obtained from a
 * centralized resource store.
 *
 * Usage example:
 * @code
 * const Music& ambientSound = resourceStore.GetMusic("audio/ambient.ogg");
 * registry.emplace<Component::LoopedAudio>(entity, ambientSound);
 * @endcode
 */
struct LoopedAudio final
{
	// ------ Members ------

	Music sound{ };
	float volume = 1.0f;


	// ------ Constructors ------

	constexpr LoopedAudio() noexcept = default;
	explicit constexpr LoopedAudio(const Music& sound) noexcept : sound(sound) { };
	constexpr LoopedAudio(const Music& sound, const float volume) noexcept : sound(sound), volume(volume) { };
};

}