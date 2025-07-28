#pragma once
#include <functional>
#include <utility>


namespace Component
{
	struct ButtonAction
	{
		std::function<void()> onClick = std::function<void()>();
		bool isActive = true;

		ButtonAction() = default;
		ButtonAction(
			std::function<void()>&& onClick
		): 
			onClick(std::move(onClick))
		{ };
	};
}