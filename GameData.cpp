#include "gameData.h"

std::map<ItemType, Item> initializeItemDatabase() {
    std::map<ItemType, Item> db;

    
    db.emplace(ItemType::CAPTAIN_KEY, Item(ItemType::CAPTAIN_KEY, "Special Key", "Key with an engraved plot number, different from the regular keys in the plot.", true));
    db.emplace(ItemType::ARMORY_KEY, Item(ItemType::ARMORY_KEY, "Heavy lead key", "A heavy lead key with a rough surface, slightly worn from use.", true));
    db.emplace(ItemType::TECH_KEY, Item(ItemType::TECH_KEY, "Tech Key", "A small key covered with a layer of plyi, most likely it had been used for a long time", true));
    db.emplace(ItemType::WORKING_FUSE, Item(ItemType::WORKING_FUSE, "Working Fuse", "Old fuse, type F, rated for 250V/1A. Traces of oxidation are visible on the contacts. Looks like it can fix the elevator power supply. ", true));

    return db;
}

std::map<LocationID, Room> initializeGameMap() {
    std::map<LocationID, Room> map;

    // --- MAIN_HALL ---
    Room hall;
    hall.name = "Main Hall";
    hall.description = "Main Hall. You see abandone hall, two staircases left and rigt and an elevator in the centre of the room. ";
    hall.exits = {
        {"Go Left", LocationID::CORRIDOR_L_1},
        {"Go Right", LocationID::CORRIDOR_R_1},
        {"Use elevator", LocationID::LABORATORY}
    };
    hall.interactables = { {"Lift", true} };
    map[LocationID::MAIN_HALL] = hall;

    // --- CORRIDOR_R_1 ---
    Room corrR1;
    corrR1.name = "Right Corridor 1F";
    corrR1.description = "This corridor leads to the Lounge Room and Armory.";
    corrR1.exits = {
        {"Go Back", LocationID::MAIN_HALL},
        {"Lounge", LocationID::ROOM_LOUNGE},
        {"Armory", LocationID::ROOM_ARMORY}
    };
    map[LocationID::CORRIDOR_R_1] = corrR1;

    // --- CORRIDOR_L_1 ---
    Room corrL1;
    corrL1.name = "Left Corridor 1F";
    corrL1.description = "This corridor leads to the Archive and Sweatbox.";
    corrL1.exits = {
        {"Go Back", LocationID::MAIN_HALL},
        {"Archive", LocationID::ROOM_ARCHIVE},
        {"SweatBox", LocationID::SWEATBOX}
    };
    map[LocationID::CORRIDOR_L_1] = corrL1;

    // --- Other 1F Rooms ---
    map[LocationID::ROOM_LOUNGE] = { "Lounge Room", "An empty lounge. In the corner stands a a heavy, metal lockers, one of them bearing the captains insignia.", {{"Exit", LocationID::CORRIDOR_R_1}} };
    map[LocationID::ROOM_LOUNGE].room_items.push_back(ItemType::CAPTAIN_KEY);
    map[LocationID::ROOM_LOUNGE].interactables = { {"Captains locker", true} };

    map[LocationID::ROOM_ARMORY] = { "Armory", "A weapon storage room. There is a metal table against the far wall.", {{"Exit", LocationID::CORRIDOR_R_1}} };
    map[LocationID::ROOM_ARMORY].room_items.push_back(ItemType::TECH_KEY);
    map[LocationID::ROOM_ARMORY].interactables = { {"Table", true} };
    map[LocationID::ROOM_ARMORY].isLocked = true;

    map[LocationID::ROOM_ARCHIVE] = { "Archive", "Dusty shelves fill the room, crammed with personnel files. One folder for 'Captain' catches your eye. There's also a shelf with 'Technical Manuals'.", {{"Exit", LocationID::CORRIDOR_L_1}} };
    map[LocationID::ROOM_ARCHIVE].interactables = { {"Captains Dossier", true}};
    map[LocationID::ROOM_ARCHIVE].interactables = { {"Tech manual", true} };
    map[LocationID::SWEATBOX] = { "Sweatbox", "A grey and dusty punishment cell with a holding cell.", {{"Exit", LocationID::CORRIDOR_L_1}} };

    // --- 2F Corridors and Rooms ---
    map[LocationID::CORRIDOR_R_2] = {
        "Right Corridor 2F",
        "Leads to the Med Bay and Office.",
        {
            {"Go Down", LocationID::MAIN_HALL},
            {"Med Bay", LocationID::ROOM_MED},
            {"Office", LocationID::ROOM_OFFICE},
            {"Cross Hall", LocationID::CORRIDOR_L_2}
        }
    };

    map[LocationID::CORRIDOR_L_2] = {
        "Left Corridor 2F",
        "Leads to the Tech Room and Captain's Cabin.",
        {
            {"Go Down", LocationID::MAIN_HALL},
            {"Tech Room", LocationID::ROOM_TECH},
            {"Captain's room", LocationID::ROOM_CAP},
            {"Cross Hall", LocationID::CORRIDOR_R_2}
        }
    };

    map[LocationID::ROOM_MED] = { "Med Bay", "A sterile medical bay. An old, bulky Electrocardiograph machine sits in the corner, unused.", {{"Exit", LocationID::CORRIDOR_R_2}} };
    map[LocationID::ROOM_MED].interactables = { {"Electrocardiograph", true}};

    map[LocationID::ROOM_OFFICE] = { "Office", "A standard office. There's a note on the desk.", {{"Exit", LocationID::CORRIDOR_R_2}} };
    map[LocationID::ROOM_OFFICE].interactables = { {"Desk", true} };

    map[LocationID::ROOM_CAP] = { "Captain's Cabin", "A lavishly furnished cabin.", {{"Exit", LocationID::CORRIDOR_L_2}} };
    map[LocationID::ROOM_CAP].interactables = { {"Captains desk", true}};
    map[LocationID::ROOM_CAP].isLocked = true;

    map[LocationID::ROOM_TECH] = { "Tech Room", "A dusty maintenance room. Requires a key.", {{"Exit", LocationID::CORRIDOR_L_2}} };
    map[LocationID::ROOM_TECH].interactables = { {"Fusebox", true}};
    map[LocationID::ROOM_TECH].isLocked = true;

    // --- FINAL ---
    map[LocationID::LABORATORY] = { "Secret Laboratory", "Where it all began, and where it must end.", {{"Go Back UP", LocationID::MAIN_HALL}} };

    return map;
}