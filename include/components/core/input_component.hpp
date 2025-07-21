#pragma once
#include <functional>
#include "raylib.h"


namespace Component
{
    struct Input 
    {
        std::function<void()> onPressed { };
        KeyboardKey key = KEY_NULL;
    };
}