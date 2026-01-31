#pragma once
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "core/data/vector2.hpp"
struct SystemContext;


namespace System::Render
{

class Radar final
{
public:
	// ------ Functions ------
	static void DrawRenderTexture(const SystemContext& context, const RenderTexture2D& radarRenderTexture);
	static void DrawRadar(const SystemContext& context, const RenderTexture2D& radarRenderTexture, Nc::Vector2f cameraPosition);

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

