#include "components/core/rendering/text_component.h"
#include "core/resource_store.h"
#include "raylib.h"
#include <string>


Font& ResourceStore::GetFont(FontStyle style)
{
    if (fontStore.find(style) == fontStore.end())
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

        fontStore.emplace(style, LoadFont(filePath.c_str()));
    }
    return fontStore.at(style);
}


Sound& ResourceStore::GetSound(const std::string& filePath)
{
	if (soundStore.find(filePath) == soundStore.end())
	{
		Sound sound = LoadSound(filePath.c_str());
		soundStore.emplace(filePath, sound);
	}
	return soundStore.at(filePath);
}
