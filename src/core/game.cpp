#include "assemblers/entities/ambient_sound_entity.hpp"
#include "assemblers/entities/move_transition_entity.hpp"
#include "assemblers/scenes/comms_scene/comms_scene.hpp"
#include "assemblers/scenes/desk_scene/desk_scene.hpp"
#include "assemblers/scenes/doorway_scene/doorway_scene.hpp"
#include "assemblers/scenes/main_menu/main_menu.hpp"
#include "assemblers/scenes/outside_scene/outside_scene.hpp"
#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "components/core/button_action_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/save.hpp"
#include "entt/entity/fwd.hpp"
#include "systems/core/lighting_system.hpp"
#include "systems/core/rendering_system.hpp"
#include "systems/ui/increment_number_system.hpp"
#include "utility/color_palette.hpp"
#include "utility/morse_code.hpp"
#include "core/debug_context.hpp"
#include "core/game.hpp"
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "core/save.hpp"
#include "cstring"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "systems/anomaly/roamer_movement_system.hpp"
#include "systems/anomaly/roamer_spawning_system.hpp"
#include "systems/core/button_action_system.hpp"
#include "systems/core/input_action_system.hpp"
#include "systems/core/input_action_system.hpp"
#include "systems/core/rendering_system.hpp"
#include "systems/core/sound_system.hpp"
#include "systems/core/tween_system.hpp"
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
#include "systems/ui/increment_number_system.hpp"
#include "utility/color_palette.hpp"
#include "utility/morse_code.hpp"
#include "utility/vector2.hpp"
#include <cmath>
#include <cstring>
#include <functional>
#include <string>
#include <utility>


#ifdef DEBUG_BUILD
DebugContext Game::debugContext { };
#endif // DEBUG_BUILD


Game::Game()
{
	InitAudioDevice();
	Save::LoadSettings(settings);
	pendingSettings = settings;
};


void Game::SetupRenderContext()
{
	const int monitor = GetCurrentMonitor();
	const Nc::Vector2i monitorSize = Nc::Vector2i(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
	renderContext.windowSize = monitorSize;

	const Nc::Vector2i displaySize = RenderContext::DISPLAY_SIZE;
	const Nc::Vector2i radarSize = RenderContext::RADAR_BOUNDS.max;

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

	LightingSystem::Initialize(renderContext.lightingContext, resourceStore);
}


#ifdef DEBUG_BUILD
void Game::SetupDebug(int args, char* argv[])
{
	for (int i = 0; i < args; ++i)
	{
		if (strcmp(argv[i], "--ignore-main-menu") == 0)
		{
			Game::debugContext.ignoreMainMenu = true;
			continue;
		};

		if (strcmp(argv[i], "--maximized-windowed") == 0)
		{
			Game::debugContext.isMaximizedWindowed = true;
			continue;
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

	MainMenu::Build(registry, gameState, resourceStore);
	SettingsMenu::Build(settings, pendingSettings, gameState, renderContext.windowSize, registry, resourceStore);

#ifdef DEBUG_BUILD
	if (!Game::debugContext.ignoreMainMenu) MainMenu::Open(registry, gameState);
	else Save::LoadGameState(gameState);

	const entt::entity entity = registry.create();

	registry.emplace<Component::UiTransform>(entity, Nc::Vector2f(0.06f, 0.9f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f(180.0f, 32.0f));
	registry.emplace<Component::Text>(entity, "SAVE STATE", RADAR_COLOR);
	
	std::function<void()> onClick = [&gameState = gameState]() { Save::SaveGameState(gameState); };
	registry.emplace<Component::ButtonAction>(entity, std::move(onClick));

#else
	MainMenu::Open(registry, gameState);
#endif // DEBUG_BUILD
}


void Game::SetupWindow() const
{
	SetConfigFlags(FLAG_VSYNC_HINT);

	Nc::Vector2i monitorSize = Nc::Vector2i(GetMonitorWidth(0), GetMonitorHeight(0));
	InitWindow(monitorSize.x, monitorSize.y, "Negative Contact");
	SetWindowState(FLAG_WINDOW_MAXIMIZED);

#ifdef DEBUG_BUILD
	SetExitKey(KEY_BACKSPACE);
	
	if (Game::debugContext.isMaximizedWindowed)
	{
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		MaximizeWindow();
	} else SetWindowState(FLAG_FULLSCREEN_MODE);
#else
	SetExitKey(KEY_NULL);
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

#ifdef DEBUG_BUILD
	constexpr Nc::Vector2f FIXED_ROAMER_SPAWN = Nc::Vector2f(0.0f, -90.0f);
	if (IsKeyPressed(KEY_PERIOD)) RoamerSpawningSystem::SpawnRoamer(registry, FIXED_ROAMER_SPAWN, gameState.anomalyState);
	
	if (IsKeyPressed(KEY_MINUS)) ++gameState.anomalyState.intensity;
	if (IsKeyPressed(KEY_EQUAL)) --gameState.anomalyState.intensity;
#endif
}


void Game::UpdateRegistries(float deltaTime)
{
	InputActionSystem::Update(registry, gameState);
	ButtonActionSystem::Update(registry, gameState, renderContext, deltaTime);
	IncrementNumberSystem::Update(registry);
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


void Game::DrawGame(float deltaTime)
{
	RadarRenderSystem::DrawRenderTexture(
		registry, renderContext.radarRenderTexture, gameState.currentScene, resourceStore
	);

	Nc::Vector2f cameraPosition = Nc::Vector2f::Zero();
	cameraPosition.x += std::cosf(gameState.time * 1.4f) * 5.0f;
	cameraPosition.y += std::sinf((gameState.time * 2.8f) - 0.3f) * 4.0f;

	BeginTextureMode(renderContext.renderTexture);
	ClearBackground(BACKGROUND_COLOR);

	// Shader& shader = resourceStore.GetShader("assets/lighting.fs");
	// BeginShaderMode(shader);

	// LightingSystem::Update(registry, renderContext.lightingContext, shader, gameState, cameraPosition, deltaTime);
	// RenderingSystem::DrawScreen(registry, renderContext, gameState, cameraPosition);
	// RadarRenderSystem::DrawRadar(
	// 	registry, renderContext.radarRenderTexture, cameraPosition, gameState.currentScene
	// );

	// EndShaderMode();
	
	//LightingSystem::Update(registry, renderContext.lightingContext, shader, gameState, cameraPosition, deltaTime);
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

	text = "DNGER LVL: " + std::to_string(gameState.anomalyState.intensity);
	DrawText(text.c_str(), 32, 148, 32, GREEN);

	DrawText("Press [/] to delete msg", 32, 240, 24, GREEN);
	DrawText("Press [.] to spawn roamer", 32, 280, 24, GREEN);
	DrawText("Press [-/=] to mod intensity", 32, 320, 24, GREEN);
}
#endif // DEBUG_BUILD
