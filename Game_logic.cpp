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
                lockMessage = u8"Дверь в оружейную наглухо заперта. Требуется тяжелый металлический ключ.";
            }
            else if (next_room_id == LocationID::ROOM_CAP) {
                keyNeeded = ItemType::CAPTAIN_KEY;
                lockMessage = u8"Комната капитана заперта. Чтобы такую открыть требуется специальный ключ";
            }
            else if (next_room_id == LocationID::ROOM_TECH) {
                keyNeeded = ItemType::TECH_KEY;
                lockMessage = u8"Дверь в техническую комнату заперта на замок. Нужен небольшой ключ для замка.";
            }

            if (keyNeeded != ItemType::NONE) {
                auto it = std::find(gameState.inventory.begin(), gameState.inventory.end(), keyNeeded);

                if (it != gameState.inventory.end())
                {
                    nextRoom.isLocked = false;
                    gameState.inventory.erase(it);
                    std::string keyName = itemDatabase.at(keyNeeded).name;
                    gameState.setSceneText(u8"Вы использовали " + keyName + ". Дверь с щелчком открылась. Теперь вход сюда открыт навсегда");
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
        gameState.setSceneText(u8"Туда нельзя.");
    }
}

void handleInteraction(
    const std::string& objectname,
    Room& currentroom,
    GameState& gamestate,
    const std::map<ItemType, Item>& itemDatabase)
{
    if (gamestate.current_room_id == LocationID::MAIN_HALL && objectname == u8"Лифт")
    {
        gamestate.setSceneText(u8"Вы бегло осмотрели панель лифта. Кнопка нижнего этажа не светится. Похоже требуется активация электропитания или специальный ключ доступа.");
        return; 
    }

    if (currentroom.interactables.count(objectname) && currentroom.interactables.at(objectname))
    {

        if (gamestate.current_room_id == LocationID::ROOM_OFFICE && objectname == u8"Стол")
        {
            gamestate.setSceneText(u8"Вы нашли записку с гербовой печатью: 'В запросе отказано. Информация запрошена неккоректно. Для ознакомления со всей информацией по участику обращайтесь в главный архив.'");
            return;
        }

        if (gamestate.current_room_id == LocationID::ROOM_LOUNGE && objectname == u8"Шкафчик капитана")
        {
            gamestate.setSceneText(u8"Шкафчик заперт на кодовый замок.");
            return;
        }

        if (!currentroom.room_items.empty())
        {
            ItemType pickedUpItemType = currentroom.room_items.back();
            gamestate.inventory.push_back(pickedUpItemType);
            currentroom.room_items.pop_back();

            std::string itemName = itemDatabase.at(pickedUpItemType).name;

            gamestate.setSceneText(u8"Вы нашли  " + itemName + ".");

            currentroom.interactables.at(objectname) = false;
        }
        else
        {
            gamestate.setSceneText(u8"Вы посмотрели на " + objectname + u8". Здесь больше ничего нету.");
        }
    }
    else
    {
        gamestate.setSceneText(u8"Вы не можете взаимодейтсвовать с этим.");
    }
}


void handlePlayerAction(
    const std::string& action,
    std::map<LocationID, Room>& gameMap,
    GameState& gameState,
    const std::map<ItemType, Item>& itemDatabase)
{
    Room& currentRoom = gameMap.at(gameState.current_room_id);

    if (action == u8"Осмотреться") {
        gameState.setSceneText(currentRoom.description);
    }
    else if (action.rfind(u8"Взаимодействовать с ", 0) == 0) {
        std::string objectname = action.substr(strlen(u8"Взаимодействовать с "));
        handleInteraction(objectname, currentRoom, gameState, itemDatabase);
    }
    else if (currentRoom.exits.count(action)) {
        handleMovement(action, currentRoom, gameState, gameMap, itemDatabase);
    }
    else {
        gameState.setSceneText(u8"Действие: " + action + u8" еще не добавлено.");
    }
}