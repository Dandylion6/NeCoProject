#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "assemblers/scenes/settings_menu/settings_background_entity.hpp"
#include "assemblers/ui/increment_setting_object.hpp"
#include "components/ui/increment_component.hpp"
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "utility/morse_code.hpp"
#include "utility/vector2.hpp"
#include <utility>


void SettingsMenuScene::Build(
    entt::registry& registry, 
    RenderContext& renderContext,
    GameState& gameState, 
    ResourceStore& resourceStore
) 
{
    Component::UiIncrement morseDotDuration = Component::UiIncrement(0.05f, 2u);
    morseDotDuration.onIncrement = [&morseSettings = gameState.morseSettings](float increment)
    {
        morseSettings.dotTime += increment;
        morseSettings.dashTime = morseSettings.dotTime * 3.0f;
        morseSettings.errorMargin = morseSettings.dotTime * MorseCode::ERROR_MARGIN;
	    morseSettings.exitTime = morseSettings.dashTime + morseSettings.errorMargin + morseSettings.dotTime;
        return morseSettings.dotTime;
    };

    Construct::IncrementSettingObject(
        Nc::Vector2f::Scale(0.5f), 
        "Morse code DOT duration",
        std::move(morseDotDuration),
        registry,
        resourceStore
    );
    Construct::SettingsBackgroundEntity(registry, gameState, renderContext.windowSize);
};