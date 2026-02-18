#pragma once
#include <string>

#include "raylib.h"
#include "entt/entity/fwd.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
struct SystemContext;


namespace Nc
{
struct RenderContext;
}


namespace System::Render
{

class Radar final
{
public:
	// ------ Functions ------
    static void DrawRenderTexture(const SystemContext& context, const RenderTexture2D& radarRenderTexture);
	static void DrawRadar(const SystemContext& context, const Nc::RenderContext& renderContext);

private:
	// ------ Types ------
	struct Context final
	{
		const SystemContext& systemContext;
		Component::Radar& radar;
		Component::Sprite& sprite;
	};


	// ------ Functions ------

	static void DrawActiveScreen(const Context& context);
	static void DrawPath(entt::registry& registry);
	static void DrawRadarArtillery(entt::registry& registry);
	static void DrawBlips(const SystemContext& context);
	static void DrawErrorWarning(const Context& context);
	static void DrawRecalibratingScreen(const Context& context);
	static std::string GetRecalibratingText(float time);
};

}

