#include "systems/ui/increment_number_system.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/ui/increment_component.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include <array>
#include <charconv>
#include <string>
#include <system_error>


void IncrementNumberSystem::Update(entt::registry& registry)
{
    auto view = registry.view<Component::UiIncrement, Component::Text>();
    for (auto [entity, increment, text] : view.each())
    {
        std::array<char, 32u> buffer;
        auto [pointer, errorCode] = std::to_chars(
            buffer.data(),
            buffer.data() + buffer.size(), 
            *increment.value, 
            std::chars_format::fixed,
            increment.decimals
        );

        if (errorCode != std::errc()) continue;
        text.text = std::string(buffer.data(), pointer);
    }
}