#pragma once
#include "components/core/rendering/text_component.h"
#include "raylib.h"
#include <string>
#include <unordered_map>


class ResourceStore
{
public:
	Font& GetFont(FontStyle style);

private:
	std::unordered_map<FontStyle, Font> fontStore { };

};