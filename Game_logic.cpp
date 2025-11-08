#include "game_logic.h"
#include <algorithm> 
#include <cstring>   



void handleMovement(
    const std::string& action,
    Room& currentRoom,
    GameState& gameState,
    std::map<LocationID, Room>& gameMap,
    const std::map<ItemType, Item>& itemDatabase)
{
    if (currentRoom.exits.count(action))
    {
        LocationID next_room_id = currentRoom.exits.at(action);
        Room& nextRoom = gameMap.at(next_room_id);

        if (nextRoom.isLocked) {
            ItemType keyNeeded = ItemType::NONE;
            std::string lockMessage = "";

            if (next_room_id == LocationID::ROOM_ARMORY) {
                keyNeeded = ItemType::ARMORY_KEY;
                lockMessage = "The Armory door is locked tight. A heavy, old key is needed.";
            }
            else if (next_room_id == LocationID::ROOM_CAP) {
                keyNeeded = ItemType::CAPTAIN_KEY;
                lockMessage = "The Captain's Cabin door is locked. You need a special key.";
            }
            else if (next_room_id == LocationID::ROOM_TECH) {
                keyNeeded = ItemType::TECH_KEY;
                lockMessage = "The door to the Tech Room is sealed. A small Tech Key is required.";
            }

            if (keyNeeded != ItemType::NONE) {
                auto it = std::find(gameState.inventory.begin(), gameState.inventory.end(), keyNeeded);

                if (it != gameState.inventory.end())
                {
                    nextRoom.isLocked = false;
                    gameState.inventory.erase(it);
                    std::string keyName = itemDatabase.at(keyNeeded).name;
                    gameState.setSceneText("You used " + keyName + ". The door clicks open and the key breaks in the lock. It is now permanently unlocked.");
                }
                else
                {
                    gameState.setSceneText(lockMessage);
                    return;
                }
            }
        }
        gameState.current_room_id = next_room_id;
        gameState.setSceneText(gameMap.at(next_room_id).description);
    }
    else {
        gameState.setSceneText("You can't go that way.");
    }
}

void handleInteraction(
    const std::string& objectname,
    Room& currentroom,
    GameState& gamestate,
    const std::map<ItemType, Item>& itemDatabase)
{
    if (gamestate.current_room_id == LocationID::MAIN_HALL && objectname == "Lift")
    {
        gamestate.setSceneText("You look closely at the central elevator panel. The button for the lowest level is covered with a service plate. It seems to require external power or a specific access key to activate.");
        return; // Возвращаемся после обработки уникального случая
    }

    // Общая логика для подбора предметов
    if (currentroom.interactables.count(objectname) && currentroom.interactables.at(objectname))
    {

        if (gamestate.current_room_id == LocationID::ROOM_OFFICE && objectname == "Desk")
        {
            gamestate.setSceneText("You find a document: 'All personal files, including senior staff, have been moved to the main Archive.'");
            return;
        }

        if (gamestate.current_room_id == LocationID::ROOM_LOUNGE && objectname == "Captains locker")
        {
            gamestate.setSceneText("The locker is sealed with a combination lock. A numeric code is required.");
            return;
        }

        if (!currentroom.room_items.empty())
        {
            ItemType pickedUpItemType = currentroom.room_items.back();
            gamestate.inventory.push_back(pickedUpItemType);
            currentroom.room_items.pop_back();

            std::string itemName = itemDatabase.at(pickedUpItemType).name;

            gamestate.setSceneText("You found a " + itemName + ".");

            // После подбора предмета, делаем объект неактивным для взаимодействия
            currentroom.interactables.at(objectname) = false;
        }
        else
        {
            gamestate.setSceneText("You look at the " + objectname + ". There is nothing else here.");
        }
    }
    else
    {
        gamestate.setSceneText("You can't interact with that.");
    }
}


void handlePlayerAction(
    const std::string& action,
    std::map<LocationID, Room>& gameMap,
    GameState& gameState,
    const std::map<ItemType, Item>& itemDatabase)
{
    Room& currentRoom = gameMap.at(gameState.current_room_id);

    if (action == "Look around") {
        gameState.setSceneText(currentRoom.description);
    }
    else if (action.rfind("Interact ", 0) == 0) {
        std::string objectname = action.substr(strlen("Interact "));
        handleInteraction(objectname, currentRoom, gameState, itemDatabase);
    }
    else if (currentRoom.exits.count(action)) {
        handleMovement(action, currentRoom, gameState, gameMap, itemDatabase);
    }
    else {
        gameState.setSceneText("Action: " + action + " - not yet implemented.");
    }
}