#pragma once
#include "components/core/rendering/text_component.hpp"
#include "raylib.h"
#include <cstdint>
#include <string>
#include <unordered_map>


class ResourceStore
{
public:
	Texture2D& GetTexture(const std::string& filePath);
	Shader& GetShader(const std::string& filePath);
	Font& GetFont(FontStyle style, uint8_t fontSize);
	Sound& GetSound(const std::string& filePath);
	Music& GetMusic(const std::string& filePath);

private:
	std::unordered_map<std::string, Texture2D> textureStore { };
	std::unordered_map<std::string, Shader> shaderStore { };
	std::unordered_map<FontKey, Font, FontKeyHash> fontStore { };
	std::unordered_map<std::string, Sound> soundStore { };
	std::unordered_map<std::string, Music> musicStore { };

};