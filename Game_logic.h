#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "structs.h"

void handlePlayerAction(
    const std::string& action,
    std::map<LocationID, Room>& gameMap,
    GameState& gameState,
    const std::map<ItemType, Item>& itemDatabase
);

#endif // GAME_LOGIC_H