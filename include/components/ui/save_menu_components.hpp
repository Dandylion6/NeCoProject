#pragma once
#include "entt/entity/fwd.hpp"


namespace Component
{
	struct SaveMenuSlot
	{
		enum PressState : uint8_t
		{
			SaveOnPress,
			LoadOnPress,
			NewGameOnPress,
			Disabled
		};
		
		entt::entity lastSavedTextEntity = entt::null;
		uint8_t slotIndex = 0u;
		PressState pressState = Disabled;
	};
}


namespace Tag
{
	struct SaveMenu { };
}