#include "core/runtime/resource_store.hpp"

#include <string>
#include <utility>

#include "raylib.h"
#include "core/data/font.hpp"

#if defined(PLATFORM_DESKTOP)
constexpr auto GLSL_VERSION = 330;
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif


namespace Nc
{

const Texture2D& ResourceStore::GetTexture(const std::string& filePath) noexcept(false)
{
	if (!textureStore.contains(filePath))
	{
		Texture texture = LoadTexture(filePath.c_str());
		textureStore.emplace(filePath, texture);
	}
	return textureStore.at(filePath);
}


const Shader& ResourceStore::GetShader(const std::string& filePath)
{
	if (!shaderStore.contains(filePath))
	{
		Shader shader = LoadShader(nullptr, TextFormat(filePath.c_str(), GLSL_VERSION));
		shaderStore.emplace(filePath, shader);
	}
	return shaderStore.at(filePath);
}


const ::Font& ResourceStore::GetFont(const Font::Style style, Font::Size fontSize) noexcept(false)
{
	FontKey key = FontKey(style, fontSize);
	if (!fontStore.contains(key))
	{
		std::string filePath{ };
		switch (style)
		{
		case Nc::Font::WDXL:
			filePath = "assets/fonts/Nc::Font::WDXLLubrifontSC-Regular.ttf";
			break;
		default:
			break;
		}
		fontStore.emplace(key, LoadFontEx(filePath.c_str(), static_cast<int32_t>(fontSize), nullptr, 0));
	}
	return fontStore.at(key);
}


Sound ResourceStore::CreateSoundHandle(const std::string& filePath) noexcept(false)
{
	const Sound& master = GetSoundMaster(filePath);
	return LoadSoundAlias(master);
}


const Music& ResourceStore::GetMusic(const std::string& filePath) noexcept(false)
{
	if (!musicStore.contains(filePath))
	{
		Music music = LoadMusicStream(filePath.c_str());
		musicStore.emplace(filePath, music);
	}
	return musicStore.at(filePath);
}


const Sound& ResourceStore::GetSoundMaster(const std::string& filePath) noexcept(false)
{
	if (!soundStore.contains(filePath))
	{
		Sound sound = LoadSound(filePath.c_str());
		soundStore.emplace(filePath, sound);
	}
	return soundStore.at(filePath);
}

}
