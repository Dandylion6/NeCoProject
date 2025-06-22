#include "assemblers/entities/move_transition_entity.h"
#include "assemblers/scenes/comms_scene/comms_scene.h"
#include "assemblers/scenes/desk_scene/desk_scene.h"
#include "assemblers/scenes/doorway_scene/doorway_scene.h"
#include "assemblers/scenes/outside_scene/outside_scene.h"
#include "core/game.h"
#include "core/game_state.h"
#include "core/render_context.h"
#include "raylib.h"
#include "systems/core/button_action_system.h"
#include "systems/core/rendering/rectangle_render_system.h"
#include "systems/core/rendering/sprite_render_system.h"
#include "systems/core/sound_system.h"
#include "systems/core/tween_system.h"
#include "systems/object/blip_blink_system.h"
#include "systems/object/morse_sound_system.h"
#include "systems/object/morse_transceiver_system.h"
#include "systems/object/radar_render_system.h"
#include "systems/object/radio_sound_system.h"
#include "systems/object/receiver/receiver_code_response_system.h"
#include "systems/object/receiver/receiver_interpreting_system.h"
#include "utility/vector2.h"
#include <cmath>


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


void Game::InitialiseAssemblers()
{
	Construct::MoveTransitionEntity(registry, renderContext, gameState);
	CommsScene::Build(registry, gameState, resourceStore);
	DeskScene::Build(registry, gameState, resourceStore);
	DoorwayScene::Build(registry, gameState, resourceStore);
	OutsideScene::Build(registry, gameState, resourceStore);
}


void Game::SetupWindow() const
{
	const Nc::Vector2i monitorSize = Nc::Vector2i(GetMonitorWidth(0), GetMonitorHeight(0));
	InitWindow(monitorSize.x, monitorSize.y, "Negative Contact");
	SetTargetFPS(GameState::FRAME_RATE);
	SetWindowState(FLAG_WINDOW_MAXIMIZED);

#ifdef _DEBUG
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	MaximizeWindow();
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
	return true;
}


void Game::Update(float deltaTime)
{
	if (IsKeyPressed(KEY_ESCAPE)) Shutdown();
	gameState.time += deltaTime;
}


void Game::UpdateRegistries(float deltaTime)
{
	ButtonActionSystem::Update(registry, gameState, renderContext, deltaTime);
	MorseTransceiverSystem::Update(registry, gameState.currentScene, deltaTime);
	MorseSoundSystem::Update(registry, gameState.currentScene, deltaTime);
	BlipBlinkSystem::Update(registry);
	ReceiverInterpretingSystem::Update(registry, resourceStore);
	ReceiverCodeResponseSystem::Update(registry, resourceStore);
	RadioSoundSystem::Update(registry, deltaTime);
	TweenSystem::Update(registry, deltaTime);
	SoundSystem::Update(registry, deltaTime);
}


void Game::DrawGame()
{
	RadarRenderSystem::DrawRenderTexture(
		registry, renderContext.radarRenderTexture, gameState.currentScene, resourceStore
	);
	DrawScreen();

	BeginDrawing();
	ClearBackground(Color(RenderContext::CLEAR_COLOR));

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

	DrawUi();
	EndDrawing();
}


void Game::DrawScreen()
{
	Nc::Vector2f cameraPosition = Nc::Vector2f::Zero();
	cameraPosition.x += std::cosf(gameState.time * 1.4f) * 5.0f;
	cameraPosition.y += std::sinf((gameState.time * 2.8f) - 0.3f) * 4.0f;

	BeginTextureMode(renderContext.renderTexture);
	ClearBackground(WHITE);

	SpriteRenderSystem::DrawScreen(registry, gameState.currentScene, cameraPosition);
	RadarRenderSystem::DrawRadar(
		registry, renderContext.radarRenderTexture, cameraPosition, gameState.currentScene
	);

	EndTextureMode();
}


void Game::DrawUi()
{
	SpriteRenderSystem::DrawUI(registry, renderContext.windowSize);
	RectangleRenderSystem::DrawUi(registry, renderContext.windowSize);
}
