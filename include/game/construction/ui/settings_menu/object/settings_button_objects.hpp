#pragma once
#include "game/contexts/scene_context.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
struct Settings;


namespace Object
{

class SettingsToMainButton final
{
public:
    static void Create(SceneContext context, Settings& settings, Settings& pendingSettings) noexcept;

};


class ApplySettingsButton final
{
public:
    static void Create(const SceneContext& context, Settings& settings, Settings& pendingSettings) noexcept;

};

}