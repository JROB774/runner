/// ENTITY HANDLER ////////////////////////////////////////////////////////////

const std::string EntityHandler::DIFFICULTY_FILE = "Resources\\Data\\Difficulty.dat";
const std::string EntityHandler::ROOM_FILE = "Resources\\Data\\Rooms.dat";
const int EntityHandler::KILL_POINT = -64;
const int EntityHandler::MEDIUM_CAP = 30, EntityHandler::HARD_CAP = 45;
std::vector <int> EntityHandler::difficulty;
int EntityHandler::currentDifficulty = EntityHandler::DIFFICULTY_EASY;
std::vector <std::string> EntityHandler::room;
int EntityHandler::currentRoomCount = 0;
int EntityHandler::roomCount = 0;
std::vector <Entity*> EntityHandler::entity;



void EntityHandler::initialise (const bool a_halloween)
{
    std::ifstream difficultyFile(DIFFICULTY_FILE);

    if (difficultyFile.is_open())
    {
        std::string data = "\0";

        while(std::getline(difficultyFile, data))
        {
            int diff = atoi(&data[0]);
            difficulty.push_back(diff);
        }

        difficultyFile.close();
    }
    else { J_Error::log("GAME_ERROR_LOAD_DIFFICULTY"); }

    std::ifstream roomFile(ROOM_FILE);

    if (roomFile.is_open())
    {
        std::string data = "\0";
        while (std::getline(roomFile, data)) { room.push_back(data); }

        roomFile.close();
    }
    else { J_Error::log("GAME_ERROR_LOAD_ROOM"); }

    currentDifficulty = DIFFICULTY_EASY;

    currentRoomCount = 0;
    roomCount = 0;

    Entity::initialise(a_halloween);

    spawn(0);

    spawn(-1, J_Window::getScreenWidth() * 2);
    spawn(-1, J_Window::getScreenWidth() * 3);
}



void EntityHandler::spawn (const int a_roomid, const int a_pos)
{
    // Setup the data necessary for room creation.
    int id = 0;

    if (a_roomid > -1) { id = (a_roomid >= static_cast <int> (room.size())) ? 0 : a_roomid; }
    else
    {
        do { id = J_Math::random(0, (int)room.size()); }
        while (difficulty.at(id) > currentDifficulty);
    }

    if (J_System::isDebug()) { printf("Spawning Room: %d (%d)\nTotal Rooms: %d\n", id, difficulty.at(id), roomCount + 1); }

    std::string rawRoomData = room.at(id);

    int entityCount = 1;
    for (unsigned int i = 0; i < rawRoomData.length(); ++i) { if (rawRoomData[i] == ' ') { ++entityCount; } }

    // If there is only one entity or less then just spawn an end entity.
    if (entityCount <= 1)
    {
        entity.push_back(nullptr);

        entity.back() = new End;
        entity.back()->create(a_pos + J_Window::getScreenWidth());

        ++currentRoomCount;

        return;
    }

    // Cut up the room into individual entities.
    char* roomData = &rawRoomData[0];
    std::string* entityData = new(std::nothrow) std::string[entityCount];
    if (entityData == nullptr) { J_Error::log("GAME_ERROR_ROOM_SPAWN"); }

    entityData[0] = strtok(roomData, " ");
    for (int i = 1; i < entityCount; ++i) { entityData[i] = strtok(nullptr, " "); }

    // Cut up the entity data into type and position.
    int* entityType = new(std::nothrow) int[entityCount];
    int* entityPos = new(std::nothrow) int[entityCount];

    if ((entityType == nullptr) || (entityPos == nullptr)) { J_Error::log("GAME_ERROR_ROOM_SPAWN"); }

    for (int i = 0; i < entityCount; ++i)
    {
        char* data = &entityData[i][0];

        entityType[i] = atoi(strtok(data, ":"));

        entityPos[i] = atoi(strtok(nullptr, ":"));
        if (entityPos[i] == -1) { entityPos[i] = J_Window::getScreenWidth(); }
    }

    // Spawn each of the entities at the correct positions.
    for (int i = 0; i < entityCount; ++i)
    {
        switch (entityType[i])
        {
            case (TYPE_END):
            {
                entity.push_back(nullptr);

                entity.back() = new(std::nothrow) End;
                if(!entity.back()) { J_Error::log("GAME_ERROR_ENTITY_SPAWN"); }
                else { entity.back()->create(a_pos + entityPos[i]); }

                break;
            }

            case (TYPE_COLLECTIBLE):
            {
                entity.push_back(nullptr);

                entity.back() = new(std::nothrow) Collectible;
                if(!entity.back()) { J_Error::log("GAME_ERROR_ENTITY_SPAWN"); }
                else { entity.back()->create(a_pos + entityPos[i]); }

                break;
            }

            case (TYPE_POLE):
            {
                entity.push_back(nullptr);

                entity.back() = new(std::nothrow) Pole;
                if(!entity.back()) { J_Error::log("GAME_ERROR_ENTITY_SPAWN"); }
                else { entity.back()->create(a_pos + entityPos[i]); }

                break;
            }

            case (TYPE_SPIKE):
            {
                entity.push_back(nullptr);

                entity.back() = new(std::nothrow) Spike;
                if(!entity.back()) { J_Error::log("GAME_ERROR_ENTITY_SPAWN"); }
                else { entity.back()->create(a_pos + entityPos[i]); }

                break;
            }

            case (TYPE_WALL):
            {
                entity.push_back(nullptr);

                entity.back() = new(std::nothrow) Wall;
                if(!entity.back()) { J_Error::log("GAME_ERROR_ENTITY_SPAWN"); }
                else { entity.back()->create(a_pos + entityPos[i]); }

                break;
            }
        }
    }

    // Clean up.
    roomData = nullptr;
    delete[] entityData;
    entityData = nullptr;
    delete[] entityType;
    entityType = nullptr;
    delete[] entityPos;
    entityPos = nullptr;

    ++currentRoomCount;
    ++roomCount;

    if (roomCount >= HARD_CAP) { currentDifficulty = DIFFICULTY_HARD; }
    else if (roomCount >= MEDIUM_CAP) { currentDifficulty = DIFFICULTY_MEDIUM; }
}



bool EntityHandler::step (const J_Collider& a_player)
{
    if (!entity.empty())
    {
        for (unsigned int i = 0; i < entity.size(); ++i) { entity.at(i)->step(); }

        for (unsigned int i = 0; i < entity.size(); ++i)
        {
            if (J_Collision::collision(a_player, entity.at(i)->getCollider()))
            {
                if (typeid(*entity.at(i)) == typeid(Collectible))
                {
                    Collectible* temp = dynamic_cast <Collectible*> (entity.at(i));
                    temp->kill();
                    temp = nullptr;
                }
                else if (typeid(*entity.at(i)) == typeid(End)) { /* Do nothing. */ }
                else { return true; }
            }
        }

        kill();
    }

    return false;
}



void EntityHandler::render ()
{
    if (!entity.empty())
    {
        for (unsigned int i = 0; i < entity.size(); ++i)
        {
            entity.at(i)->render();
        }
    }
}



int EntityHandler::getScore ()
{
    int score = 0;

    if (!entity.empty())
    {
        for (unsigned int i = 0; i < entity.size(); ++i)
        {
            if (typeid(*entity.at(i)) == typeid(Collectible))
            {
                Collectible* temp = dynamic_cast <Collectible*> (entity.at(i));
                if (temp->isDead()) { score += temp->getScore(); }
                temp = nullptr;
            }
        }
    }

    return score;
}



void EntityHandler::terminate ()
{
    if (!difficulty.empty()) { difficulty.clear(); }
    if (!room.empty()) { room.clear(); }

    if (!entity.empty())
    {
        for (unsigned int i = 0; i < entity.size(); ++i) { entity.at(i)->destroy(); }
        entity.clear();
    }

    currentDifficulty = DIFFICULTY_EASY;

    currentRoomCount = 0;
    roomCount = 0;
}



void EntityHandler::kill ()
{
    if (!entity.empty())
    {
        for (unsigned int i = 0; i < entity.size(); ++i)
        {
            if (entity.at(i)->getPosition().point.x <= KILL_POINT)
            {
                if (typeid(*entity.at(i)) == typeid(End)) { --currentRoomCount; }

                entity.at(i)->destroy();
                entity.erase(entity.begin() + i);
            }
        }

        while (currentRoomCount < 2) { spawn(); }
    }
}

/// ENTITY HANDLER ////////////////////////////////////////////////////////////
