#pragma once
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
struct SystemContext;


namespace Nc
{
struct RenderContext;
};


namespace Component::Action
{
struct Drag;
}


namespace System::Action
{

class Drag final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context, const Nc::RenderContext& renderContext);

private:
    // ------ Types ------
    enum Result
    {
        Hovering,
        NotHovering,
        Pressed
    };


    // ------ Functions ------

    static Result UpdateSceneDrag(
        const SystemContext& context,
        const Nc::RenderContext& renderContext,
        entt::entity entity,
        Component::Action::Drag& drag
    );

    static void UpdateUiDrag(const SystemContext& context, Nc::Vector2i windowSize);

};

}

