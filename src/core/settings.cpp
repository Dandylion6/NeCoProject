#include "core/settings.hpp"


void ApplyMorseSettings(MorseSettings& morseSettings)
{
    morseSettings.dashTime = morseSettings.dotTime * 3.0f;
    morseSettings.errorMargin = morseSettings.dotTime * MorseCode::ERROR_MARGIN;
	morseSettings.exitTime = morseSettings.dashTime + morseSettings.errorMargin + morseSettings.dotTime;
}


void Settings::Apply(Settings& settings)
{
    ApplyMorseSettings(settings.morseSettings);
}


void Settings::Apply(Settings& settings, Settings& toApply)
{
    settings.morseSettings.dotTime = toApply.morseSettings.dotTime;
    Apply(settings);
}