#ifndef STRUCTS_H
#define STRUCTS_H

#include "raylib.h"
#include <string>
#include <vector>
#include <map>

// --- UI Structures ---

class InventorySlot {
public:
    Rectangle rect;
    bool hovered = false;
};

class ActionButton {
public:
    Rectangle rect;
    std::string text;
    bool hovered = false;
};


enum class LocationID {
    MAIN_HALL,
    CORRIDOR_R_1, CORRIDOR_L_1, ROOM_LOUNGE, ROOM_ARMORY, ROOM_ARCHIVE, SWEATBOX,
    CORRIDOR_R_2, CORRIDOR_L_2, ROOM_MED, ROOM_OFFICE, ROOM_TECH, ROOM_CAP,
    LABORATORY,
    ERROR_LOCATION_ID
};

enum class ItemType {
    NONE = 0, CAPTAIN_KEY, ARMORY_KEY, TECH_KEY, WORKING_FUSE, SCREWDRIVER
};

// --- Game Classes ---

class Item {
public:
    ItemType id;
    std::string name;
    std::string description;
    bool consumable = false;

    Item(ItemType itemId, const std::string& itemName, const std::string& itemDesc, bool isConsumable = false)
        : id(itemId), name(itemName), description(itemDesc), consumable(isConsumable) {
    }

    Item() = default;
};

class Room {
public:
    std::string name;
    std::string description;
    std::map<std::string, LocationID> exits;
    std::vector<ItemType> room_items;
    std::map<std::string, bool> interactables;
    std::map < std::string, std::vector<ItemType> > RoomInventoryObject;
    std::map <std::string, std::string> puzzle_logic;
    bool isLocked = false;
};

class GameState {
public:
    LocationID current_room_id = LocationID::MAIN_HALL;
    std::vector<ItemType> inventory;
    std::map<std::string, bool> flags;
    std::string scenes = "You wake up in a cold, metallic hall.";

    bool isElevatorPowerRestored = false;
    bool hasLearnedTheCode = false;
    
    int visible_chars_count = 0;
    bool is_typing_finished = true;

    const std::string& getSceneText() const { return scenes; }
    void setSceneText(const std::string& newText) 
    {
        scenes = newText;
        visible_chars_count = 0;
        is_typing_finished = false;
    }
};

#endif // STRUCTS_H