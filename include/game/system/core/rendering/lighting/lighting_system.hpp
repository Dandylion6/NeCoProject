#pragma once
#include "raylib.h"
struct SystemContext;
struct GameState;


namespace Nc
{
struct RenderContext;
struct LightingContext;
class ResourceStore;
};


namespace System::Render
{
/**
 * @brief Sets up and handles passing data to the lighting shader.
 */
class Lighting final
{
public:
    // ------ Functions ------
    static void Initialize(Nc::LightingContext& context, Nc::ResourceStore& resourceStore);
    static const Shader& Update(const SystemContext& systemContext, const Nc::RenderContext& context);

private:
    // ------ Types ------
    enum LightType: int
    {
        LightPoint,
        Spotlight
    };
};

}

