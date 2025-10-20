#include "assemblers/entities/ambient_sound_entity.hpp"
#include "assemblers/entities/move_transition_entity.hpp"
#include "assemblers/menus/main_menu/main_menu.hpp"
#include "assemblers/menus/restart_menu/restart_menu.hpp"
#include "assemblers/scenes/comms_scene/comms_scene.hpp"
#include "assemblers/scenes/desk_scene/desk_scene.hpp"
#include "assemblers/scenes/doorway_scene/doorway_scene.hpp"
#include "assemblers/scenes/outside_scene/outside_scene.hpp"
#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "core/game.hpp"
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "core/save_settings.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "systems/anomaly/roamer_behaviour_system.hpp"
#include "systems/anomaly/roamer_kill_system.hpp"
#include "systems/anomaly/roamer_spawning_system.hpp"
#include "systems/core/button_action_system.hpp"
#include "systems/core/input_action_system.hpp"
#include "systems/core/lighting_system.hpp"
#include "systems/core/rendering_system.hpp"
#include "systems/core/sound_system.hpp"
#include "systems/core/tween_system.hpp"
#include "systems/object/comms/morse_monitor_display_system.hpp"
#include "systems/object/comms/morse_sound_system.hpp"
#include "systems/object/comms/morse_transceiver_system.hpp"
#include "systems/object/comms/radar/blip_blink_system.hpp"
#include "systems/object/comms/radar/blip_glitch_system.hpp"
#include "systems/object/comms/radar/radar_artillery_system.hpp"
#include "systems/object/comms/radar/radar_render_system.hpp"
#include "systems/object/comms/radar/radar_stability_system.hpp"
#include "systems/object/comms/radio_sound_system.hpp"
#include "systems/object/machine_system.hpp"
#include "systems/object/outside/blip_death_system.hpp"
#include "systems/object/outside/projectile_hit_system.hpp"
#include "systems/object/outside/receiver/artillery_aiming_systerm.hpp"
#include "systems/object/outside/receiver/artillery_fire_system.hpp"
#include "systems/object/outside/receiver/recalibrate_interpreting_system.hpp"
#include "systems/object/outside/receiver/receiver_code_response_system.hpp"
#include "systems/object/outside/receiver/receiver_interpreting_system.hpp"
#include "systems/scene/ambient_sound_system.hpp"
#include "systems/ui/increment_number_system.hpp"
#include "utility/color_palette.hpp"
#include "utility/vector2.hpp"
#include <algorithm>
#include <cmath>

#ifdef DEBUG_BUILD
#include "core/debug_context.hpp"
#include "core/save_game.hpp"
#include "components/core/button_action_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "utility/morse_code.hpp"
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>
#include <utility>
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
		}

		if (strcmp(argv[i], "--maximized-windowed") == 0)
		{
			Game::debugContext.isMaximizedWindowed = true;
			continue;
		}

		if (strcmp(argv[i], "--auto-start-radar") == 0)
		{
			Game::debugContext.isRadarActiveOnStart = true;
			continue;
		}
	}
}
#endif // DEBUG_BUILD


void Game::BuildMenuUI()
{
	MainMenu::Build(*this, registry, gameState, resourceStore);
	SettingsMenu::Build(settings, pendingSettings, gameState, renderContext.windowSize, registry, resourceStore);
	RestartMenu::Build(*this, registry, gameState, resourceStore, renderContext.windowSize);

#ifdef DEBUG_BUILD
 	if (!Game::debugContext.ignoreMainMenu) MainMenu::Open(registry, gameState);
	else
	{
		gameState.saveSlot = Save::DEBUG_SAVE_SLOT;
		Save::LoadGame(*this, registry, gameState);
	}

	const entt::entity entity = registry.create();

	registry.emplace<Component::UiTransform>(entity, Nc::Vector2f(0.06f, 0.9f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f(180.0f, 32.0f));
	registry.emplace<Component::Text>(entity, "SAVE STATE", Palette::RADAR_COLOR);
	
	std::function<void()> onClick = [&registry = registry, &gameState = gameState]() { Save::SaveGame(registry, gameState); };
	registry.emplace<Component::ButtonAction>(entity, std::move(onClick));

#else
	MainMenu::Open(registry, gameState);
#endif // DEBUG_BUILD
}


void Game::BuildRuntimeScenes()
{
	Construct::MoveTransitionEntity(registry, renderContext, gameState);
	Construct::AmbientSoundEntity(registry);

	CommsScene::Build(registry, renderContext, gameState, resourceStore);
	DeskScene::Build(registry, renderContext, gameState, resourceStore);
	DoorwayScene::Build(registry, gameState, resourceStore);
	OutsideScene::Build(registry, gameState, resourceStore);
}


void Game::SetupWindow() const
{
	SetConfigFlags(FLAG_VSYNC_HINT);

	Nc::Vector2i monitorSize = Nc::Vector2i(GetMonitorWidth(0), GetMonitorHeight(0));
	InitWindow(monitorSize.x, monitorSize.y, "Negative Contact");

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
	#ifdef DEBUG_BUILD
	constexpr Nc::Vector2f FIXED_ROAMER_SPAWN = Nc::Vector2f(0.0f, -90.0f);
	if (IsKeyPressed(KEY_PERIOD)) RoamerSpawningSystem::SpawnRoamer(registry, FIXED_ROAMER_SPAWN, gameState.anomalyState);
	
	if (IsKeyPressed(KEY_MINUS)) ++gameState.anomalyState.intensityLevel;
	if (IsKeyPressed(KEY_EQUAL)) --gameState.anomalyState.intensityLevel;
	
	if (IsKeyPressed(KEY_NINE)) gameState.anomalyState.attractionPercentage += 5.0f;
	if (IsKeyPressed(KEY_ZERO)) gameState.anomalyState.attractionPercentage -= 5.0f;

	if (IsKeyPressed(KEY_M)) Game::debugContext.timeScale += 0.5f;
	if (IsKeyPressed(KEY_N)) Game::debugContext.timeScale = std::fmaxf(Game::debugContext.timeScale - 0.5f, 0.0f);

	if (IsKeyPressed(KEY_P)) Game::Death(registry, gameState);
	#endif

	if (!gameState.isPaused)
		gameState.time += deltaTime;
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

	MorseTransceiverSystem::Update(registry, gameState, settings.morseSettings, deltaTime);
	MorseMonitorDisplaySystem::Update(registry, settings.morseSettings, deltaTime);
	MorseSoundSystem::Update(registry, gameState.currentScene, deltaTime);
	MachineSystem::Update(registry, gameState.anomalyState, deltaTime);
	RadarStabilitySystem::Update(registry, gameState.anomalyState, gameState.time, deltaTime);
	RecalibrateInterpretingSystem::Update(registry, deltaTime);
	BlipDeathSystem::Update(registry);
	BlipBlinkSystem::Update(registry);
	BlipGlitchSystem::Update(registry, gameState.time, deltaTime);
	RadarArtillerySystem::Update(registry, deltaTime);
	ReceiverInterpretingSystem::Update(registry, resourceStore);
	ReceiverCodeResponseSystem::Update(registry, resourceStore);
	RadioSoundSystem::Update(registry, deltaTime);
	ArtilleryAimingSystem::Update(registry, deltaTime);
	ArtilleryFireSystem::Update(registry, resourceStore, deltaTime);
	ProjectileHitSystem::Update(registry, deltaTime);
	RoamerSpawningSystem::Update(registry, gameState.anomalyState, gameState.time);
	RoamerBehaviourSystem::Update(registry, deltaTime);
	RoamerKillSystem::Update(registry, gameState, deltaTime);
}


void Game::LateUpdate(float deltaTime)
{
	// TODO: Attraction should be zero at down time.
	float currentAttraction = gameState.anomalyState.attractionPercentage;
	float clampedAttraction = std::clamp<float>(currentAttraction, AnomalyState::BASE_ATTRACTION, 100.0f);
	gameState.anomalyState.attractionPercentage = clampedAttraction;
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
	ClearBackground(BLANK);

	Shader& shader = resourceStore.GetShader("assets/lighting.fs");
	LightingSystem::Update(registry, renderContext.lightingContext, shader, gameState, cameraPosition, deltaTime);
	RenderingSystem::DrawScreen(registry, renderContext, gameState, cameraPosition);
	RadarRenderSystem::DrawRadar(
		registry, renderContext.radarRenderTexture, cameraPosition, gameState.currentScene
	);

	EndTextureMode();

	BeginDrawing();
	ClearBackground(Palette::BACKGROUND_COLOR);

	BeginShaderMode(shader);
	DrawRenderTexture();
	EndShaderMode();

	RenderingSystem::DrawUi(registry, resourceStore, renderContext, gameState);

#ifdef DEBUG_BUILD
	DrawDebugUi();
#endif // DEBUG_BUILD

	EndDrawing();
}


void Game::Death(entt::registry& registry, GameState& gameState)
{
	RestartMenu::Open(registry, gameState);
}


void Game::DrawRenderTexture() const
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
void Game::DrawDebugUi() const
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

	text = "DNGER LVL: " + std::to_string(gameState.anomalyState.intensityLevel);
	DrawText(text.c_str(), 32, 148, 32, GREEN);

	text = "ATRCTION: " + std::to_string(static_cast<int32_t>(gameState.anomalyState.attractionPercentage)) + "%";
	DrawText(text.c_str(), 32, 186, 32, GREEN);

	text = "RADAR: " + std::to_string(static_cast<int32_t>(Game::debugContext.radarStabilityPercentage)) + "%";
	DrawText(text.c_str(), 32, 224, 32, GREEN);

	text = "TIME: " + std::to_string(Game::debugContext.timeScale);
	DrawText(text.c_str(), 32, 272, 32, GREEN);

	DrawText("Press [/] to delete msg", 32, 340, 24, GREEN);
	DrawText("Press [.] to spawn roamer", 32, 380, 24, GREEN);
	DrawText("Press [G] to glitch a blip", 32, 420, 24, GREEN);
	DrawText("Press [P] to kill player", 32, 460, 24, GREEN);
	DrawText("Press [-/=] to mod intensity", 32, 500, 18, GREEN);
	DrawText("Press [9/0] to mod attraction", 32, 540, 18, GREEN);
	DrawText("Press [K/L] to mod radar stability", 32, 580, 18, GREEN);
	DrawText("Press [M/N] to mod time scale", 32, 620, 18, GREEN);
}
#endif // DEBUG_BUILD
