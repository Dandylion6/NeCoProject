#pragma once
#include "entt/entity/fwd.hpp"
#include <string>


class ReceiverInterpretingSystem
{
public:
	static void Update(entt::registry& registry);

private:
	static void TryInterpretMessage(std::string& message);

};