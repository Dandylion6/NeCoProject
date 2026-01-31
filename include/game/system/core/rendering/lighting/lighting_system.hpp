#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"


struct SystemContext;
struct GameState;
namespace Nc 
{ 
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
    static const Shader& Update(const SystemContext& systemContext, const Nc::LightingContext& context, Nc::Vector2f cameraPosition);

private:
    // ------ Types ------
    enum LightType: int
    {
        LightPoint,
        Spotlight
    };

};

}

