#pragma once
#include <functional>
#include <utility>


namespace Component
{
	struct ButtonAction
	{
		std::function<void()> onClick = std::function<void()>();
		bool isActive = true;
		float buttonDownTime = 0.0f;
		float lastPressedSeconds = 0.0f;

		ButtonAction() = default;
		ButtonAction(
			std::function<void()>&& onClick,
			float buttonDownTime = 0.0f
		): 
			onClick(std::move(onClick)),
			buttonDownTime(buttonDownTime)
		{ };
	};
}