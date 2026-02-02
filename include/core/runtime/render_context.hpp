#pragma once
#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/runtime/lighting_context.hpp"


namespace Nc
{

class ResourceStore;

/**
 * @brief Base internal resolution the game is rendered at before scaling to the window.
 * Used for retro-style pixel-perfect rendering.
*/
constexpr Vector2i RENDER_RESOLUTION = Vector2i(980, 740);


/**
* @brief Frame-level rendering context passed into render systems.
*
* Stores active render targets, viewport data, and lighting context for the frame.
* Stateless by design — values are set externally each frame by the runtime or system.
*/
struct RenderContext final
{
	// ------ Members ------

	RenderTexture2D renderTexture{ };
	RenderTexture2D radarRenderTexture{ };
	LightingContext lightingContext{ };
	Rectangle renderRectangle{ };
	ResourceStore& resourceStore;
	Vector2i windowSize = Vector2i::Zero();
	Vector2f cameraPosition = Vector2f::Zero();
	float renderScale = 1.0f;


	// ------ Constructors ------

	explicit constexpr RenderContext(ResourceStore& resourceStore) noexcept
		: resourceStore(resourceStore) { }
};

}
