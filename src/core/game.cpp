#include "assemblers/entities/ambient_sound_entity.hpp"
#include "assemblers/entities/move_transition_entity.hpp"
#include "assemblers/scenes/comms_scene/comms_scene.hpp"
#include "assemblers/scenes/desk_scene/desk_scene.hpp"
#include "assemblers/scenes/doorway_scene/doorway_scene.hpp"
#include "assemblers/scenes/main_menu/main_menu.hpp"
#include "assemblers/scenes/outside_scene/outside_scene.hpp"
#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "systems/core/rendering_system.hpp"
#include "utility/color_palette.hpp"
#include <cstring>
#ifdef DEBUG_BUILD
#include "core/debug_context.hpp"
#include "utility/morse_code.hpp"
#endif // DEBUG_BUILD
#include "core/game.hpp"
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "core/scene.hpp"
#include "cstring"
#include "raylib.h"
#include "systems/core/button_action_system.hpp"
#include "systems/core/input_action_system.hpp"
#include "systems/core/sound_system.hpp"
#include "systems/core/tween_system.hpp"
#include "systems/anomaly/roamer_movement_system.hpp"
#include "systems/anomaly/roamer_spawning_system.hpp"
#include "systems/object/comms/morse_monitor_display_system.hpp"
#include "systems/object/comms/morse_sound_system.hpp"
#include "systems/object/comms/morse_transceiver_system.hpp"
#include "systems/object/comms/radar/blip_blink_system.hpp"
#include "systems/object/comms/radar/radar_artillery_system.hpp"
#include "systems/object/comms/radar/radar_render_system.hpp"
#include "systems/object/comms/radio_sound_system.hpp"
#include "systems/object/outside/blip_death_system.hpp"
#include "systems/object/outside/projectile_hit_system.hpp"
#include "systems/object/outside/receiver/artillery_aiming_systerm.hpp"
#include "systems/object/outside/receiver/artillery_fire_system.hpp"
#include "systems/object/outside/receiver/receiver_code_response_system.hpp"
#include "systems/object/outside/receiver/receiver_interpreting_system.hpp"
#include "systems/scene/ambient_sound_system.hpp"
#include "systems/core/input_action_system.hpp"
#include "utility/vector2.hpp"
#include <cmath>
#include <string>


#ifdef DEBUG_BUILD
DebugContext Game::debugContext { };
#endif // DEBUG_BUILD


Game::Game()
{
	InitAudioDevice();
};


void Game::SetupRenderContext()
{
	const int monitor = GetCurrentMonitor();
	const Nc::Vector2i monitorSize = Nc::Vector2i(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
	renderContext.windowSize = monitorSize;

	const Nc::Vector2i displaySize = RenderContext::DISPLAY_SIZE;
	const Nc::Vector2i radarSize = RenderContext::RADAR_SIZE;

	renderContext.renderTexture = LoadRenderTexture(displaySize.x, displaySize.y);
	renderContext.radarRenderTexture = LoadRenderTexture(radarSize.x, radarSize.y);

	// Calculate render scale (preserve aspect ratio, clamp to nearest 0.1)
	const float scaleX = monitorSize.x / static_cast<float>(displaySize.x);
	const float scaleY = monitorSize.y / static_cast<float>(displaySize.y);
	const float scale = std::floorf(std::fminf(scaleX, scaleY) * 10.0f) * 0.1f;

	renderContext.renderScale = scale;

	// Compute scaled display size and centered position
	const Nc::Vector2i trueDisplaySize = Nc::Vector2i(displaySize * scale);
	const Nc::Vector2i origin = (monitorSize - trueDisplaySize) * 0.5f;

	renderContext.renderRectangle = {
		static_cast<float>(origin.x),
		static_cast<float>(origin.y),
		static_cast<float>(trueDisplaySize.x),
		static_cast<float>(trueDisplaySize.y)
	};
}


#ifdef DEBUG_BUILD
void Game::SetupDebug(int args, char* argv[])
{
	for (int i = 0; i < args; ++i)
	{
		if (strcmp(argv[i], "--ignore-main-menu") == 0)
		{
			Game::debugContext.ignoreMainMenu = true;
			break;
		};

		if (strcmp(argv[i], "--maximized-windowed") == 0)
		{
			Game::debugContext.isMaximizedWindowed = true;
		};
	}
}
#endif // DEBUG_BUILD


void Game::InitialiseAssemblers()
{
	Construct::MoveTransitionEntity(registry, renderContext, gameState);
	Construct::AmbientSoundEntity(registry);

	CommsScene::Build(registry, gameState, resourceStore);
	DeskScene::Build(registry, gameState, resourceStore);
	DoorwayScene::Build(registry, gameState, resourceStore);
	OutsideScene::Build(registry, gameState, resourceStore);

	SettingsMenu::Build(settings, gameState, renderContext.windowSize, registry, resourceStore);

#ifdef DEBUG_BUILD
	if (!Game::debugContext.ignoreMainMenu)
	{
		MainMenu::Build(registry, gameState, resourceStore);
		gameState.isPaused = true;
	} else gameState.currentScene = CommsRoom;
#else
	MainMenu::Build(registry, gameState, resourceStore);
	gameState.isPaused = true;
#endif // DEBUG_BUILD
}


void Game::SetupWindow() const
{
	SetConfigFlags(FLAG_VSYNC_HINT);

	Nc::Vector2i monitorSize = Nc::Vector2i(GetMonitorWidth(0), GetMonitorHeight(0));
	InitWindow(monitorSize.x, monitorSize.y, "Negative Contact");
	SetWindowState(FLAG_WINDOW_MAXIMIZED);

	SetExitKey(KEY_BACKSPACE);

#ifdef DEBUG_BUILD
	if (Game::debugContext.isMaximizedWindowed)
	{
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		MaximizeWindow();
	} else SetWindowState(FLAG_FULLSCREEN_MODE);
#else
	SetWindowState(FLAG_FULLSCREEN_MODE);
#endif
}


void Game::Shutdown()
{
	CloseAudioDevice();
	CloseWindow();
}


bool Game::ShouldRun() const
{
	if (WindowShouldClose()) return false;
	if (gameState.shouldExit) return false;
	return true;
}


void Game::Update(float deltaTime)
{
	if (!gameState.isPaused) gameState.time += deltaTime;
}


void Game::UpdateRegistries(float deltaTime)
{
	InputActionSystem::Update(registry, gameState);
	ButtonActionSystem::Update(registry, gameState, renderContext, deltaTime);
	AmbientSoundSystem::Update(registry, gameState, deltaTime);
	TweenSystem::Update(registry, gameState, deltaTime);
	SoundSystem::Update(registry, deltaTime);

	if (gameState.isPaused) return;

	MorseTransceiverSystem::Update(registry, gameState.currentScene, settings.morseSettings, deltaTime);
	MorseMonitorDisplaySystem::Update(registry, settings.morseSettings, deltaTime);
	MorseSoundSystem::Update(registry, gameState.currentScene, deltaTime);
	BlipDeathSystem::Update(registry);
	BlipBlinkSystem::Update(registry);
	RadarArtillerySystem::Update(registry, deltaTime);
	ReceiverInterpretingSystem::Update(registry, resourceStore);
	ReceiverCodeResponseSystem::Update(registry, resourceStore);
	RadioSoundSystem::Update(registry, deltaTime);
	ArtilleryAimingSystem::Update(registry, deltaTime);
	ArtilleryFireSystem::Update(registry, resourceStore, deltaTime);
	ProjectileHitSystem::Update(registry, deltaTime);
	RoamerSpawningSystem::Update(registry, gameState.anomalyState, gameState.time);
	RoamerMovementSystem::Update(registry, deltaTime);
}


void Game::DrawGame()
{
	RadarRenderSystem::DrawRenderTexture(
		registry, renderContext.radarRenderTexture, gameState.currentScene, resourceStore
	);

	Nc::Vector2f cameraPosition = Nc::Vector2f::Zero();
	cameraPosition.x += std::cosf(gameState.time * 1.4f) * 5.0f;
	cameraPosition.y += std::sinf((gameState.time * 2.8f) - 0.3f) * 4.0f;

	BeginTextureMode(renderContext.renderTexture);
	ClearBackground(BACKGROUND_COLOR);

	RenderingSystem::DrawScreen(registry, renderContext, gameState, cameraPosition);
	RadarRenderSystem::DrawRadar(
		registry, renderContext.radarRenderTexture, cameraPosition, gameState.currentScene
	);

	EndTextureMode();

	BeginDrawing();
	ClearBackground(BACKGROUND_COLOR);

	DrawRenderTexture();
	RenderingSystem::DrawUi(registry, resourceStore, renderContext, gameState);

#ifdef DEBUG_BUILD
	DrawDebugUi();
#endif // DEBUG_BUILD

	EndDrawing();
}


void Game::DrawRenderTexture()
{
	Nc::Vector2f displaySize = RenderContext::DISPLAY_SIZE;
	Rectangle source { 0, 0, displaySize.x, -displaySize.y };

	DrawTexturePro(
		renderContext.renderTexture.texture,
		source,
		renderContext.renderRectangle,
		Nc::Vector2f::Zero(),
		0.0f,
		WHITE
	);
}


#ifdef DEBUG_BUILD
void Game::DrawDebugUi()
{
	Game::debugContext.frames.pop_back();
	Game::debugContext.frames.push_front(GetFPS());
	int averageFps = 0;
	for (const int frame : Game::debugContext.frames)
	{
		averageFps += frame;
	}
	averageFps = averageFps / 32;
	std::string text = "FPS: " + std::to_string(averageFps);
	DrawText(text.c_str(), 32, 32, 32, GREEN);

	text = "MSG: " + Game::debugContext.receiverMessage;
	DrawText(text.c_str(), 32, 70, 32, GREEN);

	text = "PULSE: ";
	switch (Game::debugContext.pulse)
	{
	case MorseCode::Invalid:
		break;
	case MorseCode::Short:
		text += ".";
		break;
	case MorseCode::Long:
		text += "-";
		break;
	}
	DrawText(text.c_str(), 32, 110, 32, GREEN);
}
#endif // DEBUG_BUILD
