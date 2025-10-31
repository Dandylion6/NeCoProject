#include "utility/interpolation.hpp"
#include "utility/tween.hpp"


float Tween::GetEasing(Easing easing, float value)
{
	switch (easing)
	{
	case Linear: return value;
	case SineInOut: return Math::SineInOut(value);
	case QuadIn: return Math::QuadIn(value);
	case QuadOut: return Math::QuadOut(value);
	case CubicOut: return Math::CubicOut(value);
	case ExpoIn: return Math::ExpoIn(value);
	case ExpoOut: return Math::ExpoOut(value);
	case BackOut: return Math::BackOut(value);
	default: return value;
	}
}


void Tween::Play(Tween& tween)
{
	tween.isPlaying = true;
}


void Tween::Replay(Tween& tween)
{
	tween.elapsed = 0.0f;
	Tween::Play(tween);
}


void Tween::Build(
	float* value, 
	float start, 
	float end, 
	float duration, 
	Easing easing, 
	float delayComplete
)
{
	this->value = value;
	this->start = start;
	this->end = end;
	this->duration = duration;
	this->easing = easing;
	this->delayComplete = delayComplete;
}
