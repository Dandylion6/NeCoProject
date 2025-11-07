#pragma once
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/lighting_context.hpp"
#include "raylib.h"


namespace Nc
{

/**
 * @brief Base internal resolution the game is rendered at before scaling to the window.
 * Used for retro-style pixel-perfect rendering.
*/
constexpr Nc::Vector2i RENDER_RESOLUTION = Nc::Vector2i(980, 740);


/**
* @brief Frame-level rendering context passed into render systems.
*
* Stores active render targets, viewport data, and lighting context for the frame.
* Stateless by design — values are set externally each frame by the runtime or system.
*/
struct RenderContext final
{
	// ------ Members ------

	RenderTexture2D renderTexture { };
	RenderTexture2D radarRenderTexture { };
	LightingContext lightingContext { };
	Rectangle renderRectangle { };
	float renderScale = 1.0f;
	Nc::Vector2i windowSize = Nc::Vector2i::Zero();


	// ------ Constructors ------

	constexpr RenderContext() noexcept = default;
};

}