#include "gameData.h"

// --- База данных предметов ---
std::map<ItemType, Item> initializeItemDatabase() {
    std::map<ItemType, Item> db;

    db.emplace(ItemType::CAPTAIN_KEY, Item(ItemType::CAPTAIN_KEY, u8"Особый ключ", u8"Ключ с выгравированным номером, отличающийся от обычных.", true));
    db.emplace(ItemType::ARMORY_KEY, Item(ItemType::ARMORY_KEY, u8"Тяжелый свинцовый ключ", u8"Тяжелый свинцовый ключ с грубой поверхностью, слегка потертый.", true));
    db.emplace(ItemType::TECH_KEY, Item(ItemType::TECH_KEY, u8"Технический ключ", u8"Маленький ключ с отблеском, отпирает небольшой замок.", true));
    db.emplace(ItemType::WORKING_FUSE, Item(ItemType::WORKING_FUSE, u8"Рабочий предохранитель", u8"Старый предохранитель, тип F, на 250В/1А. На контактах видны следы окисления. Похоже, с помощью него можно восстановить электропитание лифта.", true));

    return db;
}

// --- Инициализация игровой карты ---
std::map<LocationID, Room> initializeGameMap() {
    std::map<LocationID, Room> map;

    // --- ГЛАВНЫЙ ЗАЛ ---
    Room hall;
    hall.name = u8"Главный зал";
    hall.description = u8"Главный зал. Вы видите заброшенный холл, две лестницы слева и справа, и лифт в центре комнаты.";
    hall.exits = {
        {u8"Идти налево", LocationID::CORRIDOR_L_1},
        {u8"Идти направо", LocationID::CORRIDOR_R_1},
        {u8"Использовать лифт", LocationID::CORRIDOR_R_2}
    };
    hall.interactables = { {u8"Лифтом", true} };
    map[LocationID::MAIN_HALL] = hall;

    // --- ПРАВЫЙ КОРИДОР, 1-Й ЭТАЖ ---
    Room corrR1;
    corrR1.name = u8"Правый коридор, 1-й этаж";
    corrR1.description = u8"Коридор ведет в комнату отдыха и оружейную.";
    corrR1.exits = {
        {u8"Вернуться назад", LocationID::MAIN_HALL},
        {u8"Комната отдыха", LocationID::ROOM_LOUNGE},
        {u8"Оружейная", LocationID::ROOM_ARMORY}
    };
    map[LocationID::CORRIDOR_R_1] = corrR1;

    // --- ЛЕВЫЙ КОРИДОР, 1-Й ЭТАЖ ---
    Room corrL1;
    corrL1.name = u8"Левый коридор, 1-й этаж";
    corrL1.description = u8"Коридор ведет в архив и карцер.";
    corrL1.exits = {
        {u8"Вернуться назад", LocationID::MAIN_HALL},
        {u8"Архив", LocationID::ROOM_ARCHIVE},
        {u8"Карцер", LocationID::SWEATBOX}
    };
    map[LocationID::CORRIDOR_L_1] = corrL1;

    // --- Другие комнаты, 1-й этаж ---
    map[LocationID::ROOM_LOUNGE] = { u8"Комната отдыха", u8"Пустая комната отдыха. В углу стоят тяжелые металлические шкафчики, на одном из них имя капитана.", {{u8"Выход", LocationID::CORRIDOR_R_1}} };
    map[LocationID::ROOM_LOUNGE].room_items.push_back(ItemType::CAPTAIN_KEY);
    map[LocationID::ROOM_LOUNGE].interactables = { {u8"Шкафчик капитана", true} };

    map[LocationID::ROOM_ARMORY] = { u8"Оружейная", u8"Комната для хранения оружия. У дальней стены стоит металлический стол.", {{u8"Выход", LocationID::CORRIDOR_R_1}} };
    map[LocationID::ROOM_ARMORY].room_items.push_back(ItemType::TECH_KEY);
    map[LocationID::ROOM_ARMORY].interactables = { {u8"Стол", true} };
    map[LocationID::ROOM_ARMORY].isLocked = true;

    map[LocationID::ROOM_ARCHIVE] = { u8"Архив", u8"В комнате стоит несколько пыльных стеллажей с личными делами сотрудников. Папка с надписью 'Капитан' привлекает ваше внимание. Также есть 'Технический мануал'.", {{u8"Выход", LocationID::CORRIDOR_L_1}} };
    map[LocationID::ROOM_ARCHIVE].interactables = { {u8"Досье капитана", true}, {u8"Техническое руководство", true} };
    map[LocationID::SWEATBOX] = { u8"Карцер", u8"Серая и пыльная карцерная камера.", {{u8"Выход", LocationID::CORRIDOR_L_1}} };

    // --- Коридоры и комнаты, 2-й этаж ---
    map[LocationID::CORRIDOR_R_2] = {
        u8"Правый коридор, 2-й этаж",
        u8"Ведет в медотсек и офис.",
        {
            {u8"Спуститься вниз", LocationID::MAIN_HALL},
            {u8"Медотсек", LocationID::ROOM_MED},
            {u8"Офис", LocationID::ROOM_OFFICE},
            {u8"Перейти в левый коридор", LocationID::CORRIDOR_L_2}
        }
    };

    map[LocationID::CORRIDOR_L_2] = {
        u8"Левый коридор, 2-й этаж",
        u8"Ведет в техническое помещение и кабинет капитана.",
        {
            {u8"Спуститься вниз", LocationID::MAIN_HALL},
            {u8"Тех. помещение", LocationID::ROOM_TECH},
            {u8"Каюта капитана", LocationID::ROOM_CAP},
            {u8"Перейти в правый коридор", LocationID::CORRIDOR_R_2}
        }
    };

    map[LocationID::ROOM_MED] = { u8"Медотсек", u8"Стерильный медицинский отсек. В углу стоит старый, громоздкий электрокардиограф, которым давно не пользовались.", {{u8"Выход", LocationID::CORRIDOR_R_2}} };
    map[LocationID::ROOM_MED].interactables = { {u8"Электрокардиограф", true} };

    map[LocationID::ROOM_OFFICE] = { u8"Офис", u8"Обычный кабинет. На столе лежит записка.", {{u8"Выход", LocationID::CORRIDOR_R_2}} };
    map[LocationID::ROOM_OFFICE].interactables = { {u8"Стол", true} };

    map[LocationID::ROOM_CAP] = { u8"Кабинет капитана", u8"Роскошно обставленный кабинет.", {{u8"Выход", LocationID::CORRIDOR_L_2}} };
    map[LocationID::ROOM_CAP].interactables = { {u8"Именной стол", true} };
    map[LocationID::ROOM_CAP].isLocked = true;

    map[LocationID::ROOM_TECH] = { u8"Техническое помещение", u8"Пыльная комната с электрощитками и рубильниками. Требуется ключ.", {{u8"Выход", LocationID::CORRIDOR_L_2}} };
    map[LocationID::ROOM_TECH].interactables = { {u8"Блок предохранителей", true} };
    map[LocationID::ROOM_TECH].isLocked = true;

    // --- ФИНАЛ ---
    map[LocationID::LABORATORY] = { u8"Секретная лаборатория", u8"Место, где все началось и где все должно закончиться.", {{u8"Вернуться наверх", LocationID::MAIN_HALL}} };

    return map;
}