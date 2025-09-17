#pragma once
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
		entt::registry& registry,
		const RenderTexture2D& radarRenderTexture,
		Scene currentScene,
		ResourceStore& resourceStore
	);

	static void DrawRadar(
		entt::registry& registry, 
		const RenderTexture2D& radarRenderTexture,
		Nc::Vector2f cameraPosition,
		Scene currentScene
	);

private:
	
	static Component::RadarMachine& DrawScreen(entt::registry& registry);
	static void DrawPath(entt::registry& registry);
	static void DrawRadarArtillery(entt::registry& registry);
	static void DrawBlips(entt::registry& registry, ResourceStore& resourceStore);
	static void DrawErrorWarning(
		entt::registry& registry, ResourceStore& resourceStore, Component::RadarMachine& machine
	);

};