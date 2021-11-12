#ifndef INCLUDE_ENTITY_HANDLER
#define INCLUDE_ENTITY_HANDLER

class EntityHandler
{
    public:

        static void initialise();

        static void spawn(const int = -1, const int = 240);

        static bool step(const J_Collider&);

        static void render();

        static int getScore();

        static void terminate();

    private:

        enum Type
        {
            TYPE_END,
            TYPE_COLLECTIBLE,
            TYPE_POLE,
            TYPE_SPIKE,
            TYPE_WALL,
            TYPE_TOTAL
        };

        enum Difficulty
        {
            DIFFICULTY_EASY,
            DIFFICULTY_MEDIUM,
            DIFFICULTY_HARD
        };

        static const std::string DIFFICULTY_FILE;
        static const std::string ROOM_FILE;
        static const int KILL_POINT;
        static const int MEDIUM_CAP, HARD_CAP;
        static std::vector <int> difficulty;
        static int currentDifficulty;
        static std::vector <std::string> room;
        static int currentRoomCount;
        static int roomCount;
        static std::vector <Entity*> entity;

        static void kill();
};

#endif // INCLUDE_ENTITY_HANDLER
