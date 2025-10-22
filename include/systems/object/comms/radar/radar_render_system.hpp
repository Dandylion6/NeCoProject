#pragma once
#include "components/core/rendering/sprite_component.hpp"
#include "components/objects/comms/radar.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"
class ResourceStore;


class RadarRenderSystem
{
public:
	static void DrawRenderTexture(
		entt::registry& registry, const RenderTexture2D& radarRenderTexture, Scene currentScene, ResourceStore& resourceStore
	);

	static void DrawRadar(
		entt::registry& registry, const RenderTexture2D& radarRenderTexture, Nc::Vector2f cameraPosition, Scene currentScene
	);

private:
	static void DrawActiveScreen(
		entt::registry& registry, ResourceStore& resourceStore, Component::Radar& radar, Component::Sprite& sprite
	);
	static void DrawPath(entt::registry& registry);
	static void DrawRadarArtillery(entt::registry& registry);
	static void DrawBlips(entt::registry& registry, ResourceStore& resourceStore);
	static void DrawErrorWarning(
		entt::registry& registry, ResourceStore& resourceStore, Component::Radar& radar
	);

	static void DrawRecalibratingScreen(
		entt::registry& registry, ResourceStore& resourceStore, Component::Radar& radar, Component::Sprite& sprite
	);

};