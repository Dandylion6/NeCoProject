#pragma once
#include "components/core/rendering/text_component.h"
#include "raylib.h"
#include <string>
#include <unordered_map>


class ResourceStore
{
public:
	Font& GetFont(FontStyle style);
	Sound& GetSound(const std::string& filePath);

private:
	std::unordered_map<FontStyle, Font> fontStore { };
	std::unordered_map<std::string, Sound> soundStore { };

};