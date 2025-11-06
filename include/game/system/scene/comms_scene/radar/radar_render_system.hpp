#pragma once
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "core/data/vector2.hpp"
namespace Nc { class ResourceStore; };


class RadarRenderSystem
{
public:
	static void DrawRenderTexture(
		entt::registry& registry, const RenderTexture2D& radarRenderTexture, Scene currentScene, Nc::ResourceStore& resourceStore
	);

	static void DrawRadar(
		entt::registry& registry, const RenderTexture2D& radarRenderTexture, Nc::Vector2f cameraPosition, Scene currentScene
	);

private:
	static void DrawActiveScreen(
		entt::registry& registry, Nc::ResourceStore& resourceStore, Component::Radar& radar, Component::Sprite& sprite
	);
	static void DrawPath(entt::registry& registry);
	static void DrawRadarArtillery(entt::registry& registry);
	static void DrawBlips(entt::registry& registry, Nc::ResourceStore& resourceStore);
	static void DrawErrorWarning(
		entt::registry& registry, Nc::ResourceStore& resourceStore, Component::Radar& radar
	);

	static void DrawRecalibratingScreen(
		entt::registry& registry, Nc::ResourceStore& resourceStore, Component::Radar& radar, Component::Sprite& sprite
	);

};