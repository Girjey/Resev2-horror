
#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "structs.h"

std::map<ItemType, Item> initializeItemDatabase();
std::map<LocationID, Room> initializeGameMap();

#endif // GAME_DATA_H