#pragma once
#include "core/data/vector2.hpp"
struct SystemContext;


namespace Nc
{
struct RenderContext;
}


namespace System::Action
{

class Click final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context, const Nc::RenderContext& renderContext);

private:
	// ------ Functions ------
	static void UpdateSceneButtons(const SystemContext& context, const Nc::RenderContext& renderContext);
	static void UpdateUiButtons(const SystemContext& context, Nc::Vector2i windowSize);

};

}
