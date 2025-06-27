#pragma once


namespace Component
{
	struct Blip
	{
		enum Tweens
		{
			BlipFadeIn,
			BlipFadeOut
		};

		bool isActive = true;
	};
}