#include "components/core/rendering/text_component.hpp"
#include "core/resource_store.hpp"
#include "raylib.h"
#include <string>
#include <utility>


#if defined(PLATFORM_DESKTOP)
constexpr auto GLSL_VERSION = 330;
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


Texture2D& ResourceStore::GetTexture(const std::string& filePath)
{
	if (textureStore.find(filePath) == textureStore.end())
	{
		Texture texture = LoadTexture(filePath.c_str());
		textureStore.emplace(filePath, std::move(texture));
	}
	return textureStore.at(filePath);
}


Shader& ResourceStore::GetShader(const std::string& filePath)
{
    if (shaderStore.find(filePath) == shaderStore.end())
	{
		Shader shader = LoadShader(0, TextFormat(filePath.c_str(), GLSL_VERSION));
		shaderStore.emplace(filePath, std::move(shader));
	}
	return shaderStore.at(filePath);
}


Font& ResourceStore::GetFont(FontStyle style, uint8_t fontSize)
{
    FontKey key = FontKey(style, fontSize);
    if (fontStore.find(key) == fontStore.end())
    {
        std::string filePath { };
        switch (style)
        {
        case WDXL: 
            filePath = "assets/fonts/WDXLLubrifontSC-Regular.ttf";
            break;
        default: 
            break;
        }

        fontStore.emplace(
            key, LoadFontEx(filePath.c_str(), fontSize, nullptr, 0)
        );
    }
    return fontStore.at(key);
}


Sound& ResourceStore::GetSound(const std::string& filePath)
{
	if (soundStore.find(filePath) == soundStore.end())
	{
		Sound sound = LoadSound(filePath.c_str());
		soundStore.emplace(filePath, std::move(sound));
	}
	return soundStore.at(filePath);
}
