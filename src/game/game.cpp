#include "game/game.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/comms_desk_scene/comms_desk_scene.hpp"
#include "game/construction/scene/comms_scene/comms_scene.hpp"
#include "game/construction/scene/doorway_scene/doorway_scene.hpp"
#include "game/construction/scene/outside_scene/outside_scene.hpp"
#include "game/construction/shared/entity/environment/ambient_sound_entity.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/restart_menu/restart_menu.hpp"
#include "game/construction/ui/settings_menu/settings_menu.hpp"
#include "game/construction/ui/shared/entity/move_transition_entity.hpp"
#include "game/save/save_settings.hpp"
#include "game/state/game_state.hpp"
#include "game/system/core/audio/ambient_sound_system.hpp"
#include "game/system/core/audio/sound_emitter_system.hpp"
#include "game/system/core/interactive/button_action_system.hpp"
#include "game/system/core/interactive/drag_action_system.hpp"
#include "game/system/core/interactive/input_action_system.hpp"
#include "game/system/core/rendering/lighting/lighting_system.hpp"
#include "game/system/core/rendering/rendering_system.hpp"
#include "game/system/core/tween_system.hpp"
#include "game/system/scene/comms_scene/morse_code/morse_monitor_display_system.hpp"
#include "game/system/scene/comms_scene/morse_code/morse_sound_system.hpp"
#include "game/system/scene/comms_scene/morse_code/morse_transceiver_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_blink_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_death_system.hpp"
#include "game/system/scene/comms_scene/radar/blip/blip_glitch_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_artillery_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_render_system.hpp"
#include "game/system/scene/comms_scene/radar/radar_stability_system.hpp"
#include "game/system/scene/comms_scene/radio/radio_sound_system.hpp"
#include "game/system/scene/outside_scene/artillery/artillery_aiming_system.hpp"
#include "game/system/scene/outside_scene/artillery/projectile_hit_system.hpp"
#include "game/system/scene/outside_scene/receiver/fire_interpreting_system.hpp"
#include "game/system/scene/outside_scene/receiver/recalibrate_interpreting_system.hpp"
#include "game/system/scene/outside_scene/receiver/receiver_code_response_system.hpp"
#include "game/system/scene/outside_scene/receiver/receiver_interpreting_system.hpp"
#include "game/system/shared/anomaly/anomaly_attraction_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_behaviour_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_kill_system.hpp"
#include "game/system/shared/anomaly/roamer/roamer_spawning_system.hpp"
#include "game/system/shared/mechanical/circuit_breaker_system.hpp"
#include "game/system/shared/mechanical/lever_system.hpp"
#include "game/system/shared/mechanical/machine_system.hpp"
#include "game/system/ui/interactive/increment_number_system.hpp"
#include "game/utility/color_palette.hpp"
#include "raylib.h"
#include <cmath>

#ifdef DEBUG_BUILD
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/debug/debug_context.hpp"
#include "game/save/save_game.hpp"
#include "game/state/scene.hpp"
#include "game/utility/morse_code.hpp"
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>
#include <utility>
DebugContext Game::debugContext{};
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

	const Nc::Vector2i displaySize = Nc::RENDER_RESOLUTION;
	const Nc::Vector2i radarSize = RADAR_BOUNDS.max;

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

		if (strcmp(argv[i], "--start-at-night") == 0)
		{
			gameState.hour = GameState::NIGHT_RANGE.x;
		}
	}
}
#endif // DEBUG_BUILD


void Game::BuildMenuUI()
{
	Nc::Vector2f windowSize = Nc::Vector2f(renderContext.windowSize);

	Structure::MainMenu::Build(registry, resourceStore, *this, gameState);
	Structure::SettingsMenu::Build(registry, resourceStore, settings, pendingSettings, gameState, windowSize);
	Structure::RestartMenu::Build(registry, resourceStore, *this, gameState, windowSize);

#ifdef DEBUG_BUILD
 	if (!Game::debugContext.ignoreMainMenu) 
		Structure::MainMenu::Open(registry, gameState);
	else
	{
		// Load a mock game state for testing.
		gameState.currentScene = CommsRoom;
		BuildRuntimeScenes();
	}

	const entt::entity entity = registry.create();

	registry.emplace<Component::UI::Transform>(entity, Nc::Vector2f(0.06f, 0.9f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f(180.0f, 32.0f));
	registry.emplace<Component::Text>(entity, "SAVE STATE", Palette::RADAR_COLOR);
	
	std::function<void()> onClick = [&registry = registry, &gameState = gameState]() { Save::SaveGame(registry, gameState); };
	registry.emplace<Component::Action::Click>(entity, std::move(onClick));

#else
	MainMenu::Open(registry, gameState);
#endif // DEBUG_BUILD
}


void Game::BuildRuntimeScenes()
{
	Entity::MoveTransition::Create(registry, renderContext, gameState);
	Entity::AmbientSound::Create(registry);

	Structure::CommsScene::Build(registry, resourceStore, renderContext, gameState);
	Structure::DeskScene::Build(registry, resourceStore, renderContext, gameState);
	Structure::DoorwayScene::Build(registry, resourceStore, renderContext, gameState);
	Structure::OutsideScene::Build(registry, resourceStore, gameState);
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
	if (IsKeyPressed(KEY_PERIOD))
	{
		Nc::Vector2f spawnPoint = RoamerSpawningSystem::GenerateRandomSpawnPoint();
		RoamerSpawningSystem::SpawnRoamer(registry, resourceStore, spawnPoint, gameState.anomalyState);
	}
	
	if (IsKeyPressed(KEY_MINUS)) ++gameState.anomalyState.intensityLevel;
	if (IsKeyPressed(KEY_EQUAL)) --gameState.anomalyState.intensityLevel;
	
	if (IsKeyPressed(KEY_NINE)) gameState.anomalyState.attractionPercentage += 5.0f;
	if (IsKeyPressed(KEY_ZERO)) gameState.anomalyState.attractionPercentage -= 5.0f;

	if (IsKeyPressed(KEY_M)) Game::debugContext.timeScale += 0.5f;
	if (IsKeyPressed(KEY_N)) Game::debugContext.timeScale = std::fmaxf(Game::debugContext.timeScale - 0.5f, 0.0f);

	if (IsKeyPressed(KEY_P)) Game::Death(registry, gameState);
	#endif

	if (gameState.isPaused) return;
	gameState.time += deltaTime;

	if (gameState.survivedNight) return;

	constexpr float HOUR_INCREASE_RATE = 1.0f / (GameState::HOUR_MINUTES * 60.0f);
	constexpr float HOURS_IN_DAY = 24.0f;

	float oldHour = gameState.hour;
	float newHour = std::fmodf(gameState.hour + HOUR_INCREASE_RATE * deltaTime, HOURS_IN_DAY);
	gameState.hour = newHour;

	float nightEndHour = GameState::NIGHT_RANGE.x + 1.0f;
	if (oldHour < nightEndHour && newHour >= nightEndHour)
		gameState.survivedNight = true;
}


void Game::UpdateRegistries(float deltaTime)
{
	InputActionSystem::Update(registry, gameState);
	bool buttonHovering = ClickSystem::Update(registry, gameState, renderContext);
	bool dragHovering = DragActionSystem::Update(registry, gameState, renderContext);
	IncrementNumberSystem::Update(registry);
	AmbientSoundEmitterSystem::Update(registry, gameState, resourceStore, deltaTime);
	TweenSystem::Update(registry, gameState, deltaTime);
	SoundEmitterSystem::Update(registry, deltaTime);

	MouseCursor cursor = (buttonHovering || dragHovering) ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT;
	SetMouseCursor(cursor);

	if (gameState.isPaused) return;

	MorseTransceiverSystem::Update(registry, gameState, settings.morseSettings, deltaTime);
	MorseMonitorDisplaySystem::Update(registry, settings.morseSettings, deltaTime);
	MorseSoundEmitterSystem::Update(registry, gameState.currentScene, deltaTime);
	MachineSystem::Update(registry, gameState.anomalyState, deltaTime);
	RadarStabilitySystem::Update(registry, gameState, gameState.time, deltaTime);
	RecalibrateInterpretingSystem::Update(registry, deltaTime);
	BlipDeathSystem::Update(registry);
	BlipBlinkSystem::Update(registry);
	BlipGlitchSystem::Update(registry, gameState.time, deltaTime);
	RadarArtillerySystem::Update(registry, deltaTime);
	ReceiverInterpretingSystem::Update(registry, resourceStore);
	ReceiverCodeResponseSystem::Update(registry, resourceStore);
	RadioSoundEmitterSystem::Update(registry, deltaTime);
	LeverSystem::Update(registry, deltaTime);
	CircuitBreakerSystem::Update(registry, gameState.anomalyState, deltaTime);
	ArtilleryAimingSystem::Update(registry, deltaTime);
	FireInterpretingSystem::Update(registry, resourceStore, deltaTime);
	ProjectileHitSystem::Update(registry, deltaTime);
	RoamerSpawningSystem::Update(registry, resourceStore, gameState, deltaTime);
	RoamerBehaviourSystem::Update(registry, gameState.anomalyState, deltaTime);
	RoamerKillSystem::Update(registry, gameState, deltaTime);
	AnomalyAttractionSystem::Update(gameState, deltaTime);
}


void Game::DrawGame(float deltaTime)
{
	RadarRenderSystem::DrawRenderTexture(
		registry, renderContext.radarRenderTexture, gameState.currentScene, resourceStore
	);

	Nc::Vector2f cameraPosition = Nc::Vector2f::Zero();
	cameraPosition.x = std::cosf(gameState.time * 0.6f) * 5.0f;
	cameraPosition.y = std::sinf((gameState.time * 2.4f) - 0.2f) * 4.0f;

	BeginTextureMode(renderContext.renderTexture);
	ClearBackground(BLANK);

	const Shader& shader = resourceStore.GetShader("assets/lighting.fs");
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
	Structure::RestartMenu::Open(registry, gameState);
}


void Game::DrawRenderTexture() const
{
	Nc::Vector2f displaySize = Nc::Vector2f(Nc::RENDER_RESOLUTION);
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

	text = "TIME SPD: " + std::to_string(Game::debugContext.timeScale);
	DrawText(text.c_str(), 32, 272, 32, GREEN);

	text = "DAY & H: " + std::to_string(gameState.day) + " / " + std::to_string(static_cast<int32_t>(gameState.hour));
	DrawText(text.c_str(), 32, 320, 32, GREEN);

	DrawText("Press [/] to delete msg", 32, 540, 24, GREEN);
	DrawText("Press [.] to spawn roamer", 32, 580, 24, GREEN);
	DrawText("Press [G] to glitch a blip", 32, 620, 24, GREEN);
	DrawText("Press [P] to kill player", 32, 660, 24, GREEN);
	DrawText("Press [-/=] to mod intensity", 32, 700, 18, GREEN);
	DrawText("Press [9/0] to mod attraction", 32, 740, 18, GREEN);
	DrawText("Press [K/L] to mod radar stability", 32, 780, 18, GREEN);
	DrawText("Press [M/N] to mod time scale", 32, 820, 18, GREEN);
}
#endif // DEBUG_BUILD
