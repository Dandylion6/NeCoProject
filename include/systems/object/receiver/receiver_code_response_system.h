#pragma once
#include "components/core/sound_emitter_component.h"
#include "components/objects/receiver_component.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
class ResourceStore;


class ReceiverCodeResponseSystem
{
public:
	static void Update(
		entt::registry& registry,
		ResourceStore& resourceStore
	);

private:
	static void RespondToCharacterCode(
		entt::registry& registry,
		Component::Receiver& receiver,
		ResourceStore& resourceStore
	);

};