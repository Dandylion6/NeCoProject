#pragma once
#include "core/data/font.hpp"
#include "raylib.h"
#include <string>
#include <unordered_map>


namespace Nc
{
/**
 * @brief Centralized cache and owner of all loaded Raylib resources.
 *
 * The ResourceStore manages the lifetime of textures, shaders, fonts, sounds, and music streams.
 * It ensures each resource is loaded only once and provides read-only access to the shared instance.
 *
 * In an ECS context, components should copy the returned objects (e.g., Texture2D or Sound)
 * into their own component data rather than storing references. Raylib internally uses lightweight
 * handles for these types, so copying is cheap and safe.
 */
class ResourceStore final
{
public:
	/**
	 * @brief Retrieves or loads a texture by file path.
	 *
	 * If the texture is not already cached, it will be loaded and stored internally.
	 * The returned reference is constant.
	 *
	 * @param filePath Path to the texture file.
	 * @return Const reference to the cached Texture2D. Safe to copy into components.
	 */
	const Texture2D& GetTexture(const std::string& filePath) noexcept(false);

	/**
	 * @brief Retrieves or loads a shader by file path.
	 *
	 * If not yet cached, the shader is loaded and compiled before being stored.
	 *
	 * @param filePath Path to the shader file.
	 * @return Const reference to the cached Shader.
	 */
	const Shader& GetShader(const std::string& filePath);

	const ::Font& GetFont(Font::Style style, Font::Size fontSize) noexcept(false);

	/**
	 * @brief Retrieves or loads a sound effect by file path.
	 *
	 * Cached sounds can be safely reused by copying the returned handle into
	 * audio-related components.
	 *
	 * @param filePath Path to the sound file.
	 * @return Const reference to the cached Sound instance.
	 */
	const Sound& GetSound(const std::string& filePath) noexcept(false);

	/**
	 * @brief Retrieves or loads a music stream by file path.
	 *
	 * Music streams are long-form audio assets typically streamed from disk.
	 * The returned reference is read-only.
	 *
	 * @param filePath Path to the music file.
	 * @return Const reference to the cached Music instance.
	 */
	const Music& GetMusic(const std::string& filePath) noexcept(false);

private:
	std::unordered_map<std::string, Texture2D> textureStore { };
	std::unordered_map<std::string, Shader> shaderStore { };
	std::unordered_map<FontKey, ::Font, FontKeyHash> fontStore { };
	std::unordered_map<std::string, Sound> soundStore { };
	std::unordered_map<std::string, Music> musicStore { };

};

}