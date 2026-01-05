#pragma once
#include "raylib.h"
#include <utility>

namespace Component
{
	/**
	 * @brief Component that allows an entity to emit a one-shot sound.
	 *
	 * This component represents a sound that can be played once or on demand,
	 * such as gunshots, footsteps, or UI clicks.
	 *
	 * The sound data is stored directly as a `Sound` object from raylib, meaning
	 * it should be preloaded or retrieved from a resource store to avoid
	 * repeated loading. The `volume` value scales playback volume (1.0f = full volume).
	 *
	 * Usage example:
	 *
	 * ```cpp
	 * Sound gunshotSound = resourceStore.GetSound("audio/gunshot.wav");
	 * registry.emplace<Component::SoundEmitter>(entity, std::move(gunshotSound));
	 * ```
	 */
	struct SoundEmitter final
	{
		// ------ Members ------

		Sound sound { };
		float volume = 1.0f;

		// ------ Constructors ------

		constexpr SoundEmitter() noexcept = default;
		constexpr SoundEmitter(Sound &&sound) noexcept : 
			sound(std::move(sound)) 
		{ };
	};

	/**
	 * @brief Component that allows an entity to continuously emit looping sound or music.
	 *
	 * Designed for background audio or continuous sound effects such as ambience,
	 * engine hums, or music tracks. The underlying `Music` object in raylib handles
	 * streaming automatically, making it suitable for large or long audio files.
	 *
	 * Like `SoundEmitter`, the sound should be preloaded or obtained from a
	 * centralized resource store.
	 *
	 * Usage example:
	 *
	 * ```cpp
	 * Music ambientSound = resourceStore.GetMusic("audio/ambient.ogg");
	 * entity.add<Component::LoopedSoundEmitter>(std::move(ambientSound));
	 * ```
	 */
	struct LoopedSoundEmitter final
	{
		// ------ Members ------

		Music sound { };
		float volume = 1.0f;


		// ------ Constructors ------

		constexpr LoopedSoundEmitter() noexcept = default;
		constexpr LoopedSoundEmitter(Music &&sound) noexcept : 
			sound(std::move(sound)) 
		{ };

		constexpr LoopedSoundEmitter(
			Music &&sound, float volume
		) noexcept : 
			sound(std::move(sound)), 
			volume(volume) 
		{ };
	};

}