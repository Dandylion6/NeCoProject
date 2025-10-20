#include "components/objects/comms/radar.hpp"
#include "components/objects/machine.hpp"
#include "components/scene/address_component.hpp"
#include "components/scene/dont_destroy_on_load_tag.hpp"
#include "core/game.hpp"
#include "core/game_state.hpp"
#include "core/save_game.hpp"
#include "core/scene.hpp"
#include "corecrt.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#ifdef RELEASE_BUILD
#include "base64.hpp"
#endif // RELEASE_BUILD

// @todo: Error handling for file I/O and JSON parsing.


namespace Save
{
    static SaveResult SaveGameState(GameState& gameState, nlohmann::json& data)
    {
        nlohmann::json& gameStateData = data["game_state"];
        gameStateData["attraction_percentage"] = gameState.anomalyState.attractionPercentage;
        gameStateData["time"] = gameState.time;
        gameStateData["current_scene"] = static_cast<uint8_t>(gameState.currentScene);
        return SaveResult::Success;
    }


    static LoadResult LoadGameState(GameState& gameState, nlohmann::json& data)
    {
        if (!data.contains("game_state"))
        {
            // Load starting data
            nlohmann::json& gameStateData = data["game_state"];
            gameStateData["attraction_percentage"] = AnomalyState::BASE_ATTRACTION;
            gameStateData["time"] = 0.0f;
            gameStateData["current_scene"] = static_cast<uint8_t>(CommsRoom);
        }
        
        nlohmann::json& gameStateData = data["game_state"];
        gameState.anomalyState.attractionPercentage = gameStateData["attraction_percentage"];
        gameState.time = gameStateData["time"];
        gameState.currentScene = static_cast<Scene>(gameStateData["current_scene"]);
        return LoadResult::Success;
    }


    static SaveResult SaveMachineComponents(entt::registry& registry, nlohmann::json& data)
    {
        auto view = registry.view<const Component::Address, const Component::Machine>();
        for (auto [entity, address, machine] : view.each())
        {
            if (address.address.empty()) return SaveResult::Failure;

            nlohmann::json& entityData = data["entities"][address.address];

            entityData["machine_is_active"] = machine.isActive;

            // In the case where the machine has other relevant components
            if (registry.any_of<Component::RadarMachine>(entity))
            {
                Component::RadarMachine& radar = registry.get<Component::RadarMachine>(entity);
                entityData["radar_stability"] = radar.stability;
                entityData["radar_recalibration_time"] = radar.recalibrationTimeLeft;
            }
        }
        return SaveResult::Success;
    }


    static LoadResult LoadMachineComponents(entt::registry& registry, nlohmann::json& data)
    {
        nlohmann::json& entitiesData = data["entities"];
        auto view = registry.view<Component::Address, Component::Machine>();
        for (auto [entity, address, machine] : view.each())
        {
            if (!entitiesData.contains(address.address)) return LoadResult::Failure;
            
            nlohmann::json& entityData = entitiesData[address.address];
            if (!entityData.contains("machine_is_active")) return LoadResult::Failure;
            machine.isActive = entityData["machine_is_active"];

            if (registry.any_of<Component::RadarMachine>(entity))
            {
                if (!entityData.contains("radar_stability")) return LoadResult::Failure;
                if (!entityData.contains("radar_recalibration_time")) return LoadResult::Failure;

                Component::RadarMachine& radar = registry.get<Component::RadarMachine>(entity);
                radar.stability = entityData["radar_stability"];
                radar.recalibrationTimeLeft = entityData["radar_recalibration_time"];
            }
        }
        return LoadResult::Success;
    }


    static nlohmann::json GetMetaDataJson(std::filesystem::path metaFilePath)
    {
        nlohmann::json data;
        if (std::filesystem::exists(metaFilePath))
        {
            std::ifstream inputStream(metaFilePath);
            try {
                inputStream >> data;
                inputStream.close();
				return data;
            }
            catch (nlohmann::json::parse_error& e)
            {
                return nlohmann::json::object();
            }
        }
        return nlohmann::json::object();
    }
}


Save::SaveResult Save::SaveGame(entt::registry& registry, SaveContext& saveContext, GameState& gameState)
{
	if (saveContext.currentSaveSlot > SaveContext::MAX_SAVE_SLOTS) 
        return SaveResult::Failure; // Invalid save slot.

    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);

	std::string fileName = "slot_" + std::to_string(saveContext.currentSaveSlot);
    std::ofstream stream(dataDirectoryPath / (fileName + ".save"), std::ios::out);
    nlohmann::json data;

    Save::SaveGameState(gameState, data);
    Save::SaveMachineComponents(registry, data);

#ifdef DEBUG_BUILD
    stream << data.dump(4) << std::endl;
#else
    std::string dataString = data.dump();
    std::string encodedData = base64::to_base64(dataString);
    stream << encodedData;
#endif
    stream.close();
    return SaveResult::Success;
}


Save::LoadResult Save::LoadGame(Game& game, entt::registry& registry, SaveContext& saveContext, GameState& gameState)
{
	if (saveContext.currentSaveSlot > SaveContext::MAX_SAVE_SLOTS) 
        return LoadResult::Failure; // Invalid save slot.

    // Cleans up the game before rebuilding
    for (const entt::entity entity : registry.view<const entt::entity>())
    {
        if (registry.any_of<Tag::DontDestroyOnLoad>(entity)) continue; // Ignores the entity
        registry.destroy(entity);
    }
    game.BuildRuntimeScenes();

    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);

    std::string fileName = "slot_" + std::to_string(saveContext.currentSaveSlot);
    std::ifstream stream(dataDirectoryPath / (fileName + ".save"), std::ios::in);
    
#ifdef DEBUG_BUILD
    nlohmann::json data { };
    if (stream.is_open()) data = nlohmann::json::parse(stream);
    stream.close();
#else
    std::stringstream stringBuffer;
    stringBuffer << stream.rdbuf();
    
    std::string decodedData = base64::from_base64(stringBuffer.view());
    nlohmann::json data;
    if (!decodedData.empty()) data = nlohmann::json::parse(decodedData);
#endif

    Save::LoadGameState(gameState, data);
    Save::LoadMachineComponents(registry, data);

    return LoadResult::Success;
}


Save::SaveResult Save::SaveMetaData(SaveContext& saveContext, GameState& gameState)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::filesystem::path metaFilePath = dataDirectoryPath / "saves.metadata";

	nlohmann::json metaData = Save::GetMetaDataJson(metaFilePath);
    nlohmann::json slotData = metaData["save_slots"][std::to_string(saveContext.currentSaveSlot)];
	slotData["last_saved_time"] = std::time(nullptr);
	// @todo: Add more metadata fields as needed.

    std::ofstream outputStream(metaFilePath, std::ios::out | std::ios::trunc);
    outputStream << metaData.dump(4);

    outputStream.close();
    return SaveResult::Success;
}


Save::LoadResult Save::LoadMetaData(SaveContext& saveContext, GameState& gameState)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    
	nlohmann::json metaData = Save::GetMetaDataJson(dataDirectoryPath / "saves.metadata");
    
    for (uint8_t slot = 0u; slot <= SaveContext::MAX_SAVE_SLOTS; ++slot)
    {
        if (!metaData["save_slots"].contains(std::to_string(slot))) continue;
        
		nlohmann::json slotData = metaData["save_slots"][std::to_string(slot)];
        saveContext.saveMetaData[slot].isUsed = true;
		saveContext.saveMetaData[slot].lastSavedTime = static_cast<time_t>(slotData["last_saved_time"]);
    }

    return LoadResult::Success;
}