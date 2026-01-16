#pragma once
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"


namespace entt
{

template<class... Cs>
entity get_single(registry& reg) {
    auto view = reg.view<Cs...>();
    auto it = view.begin();

#ifdef DEBUG_BUILD
    assert(it != view.end() && "Single entity is missing!");
#endif

    return (it == view.end()) ? null : *it;
}

}