#include "game/system/ui/interactive/increment_value_system.hpp"

#include <array>
#include <string>

#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/ui/increment_component.hpp"


void System::UI::IncrementValue::Update(entt::registry& registry)
{
    const auto view = registry.view<Component::UI::Increment, Component::Text>();
    for (auto [entity, increment, text] : view.each())
    {
        const float value = *increment.value;

        std::array<char, 16u> buffer{ };
        const int bytes = snprintf(buffer.data(), buffer.size(), "%.*f", increment.decimals, value);

        if (bytes >= 0 && bytes < buffer.size())
            text.text.append(buffer.data(), bytes);
    }
}
