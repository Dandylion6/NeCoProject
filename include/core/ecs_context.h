#pragma once
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "entt/signal/dispatcher.hpp"
#include "entt/signal/fwd.hpp"


struct EcsContext
{
	entt::registry registry { };
	entt::dispatcher dispatcher { };
};