#ifndef INCLUDE_SAVE
#define INCLUDE_SAVE

class Save
{
    public:

        static void step();
        static void reset();
        static void load();
        static void save();

    private:

        static const std::string SAVE_FILE;

        static const bool DEFAULT_FULLSCREEN;
        static const float DEFAULT_VOLUME;
        static const bool DEFAULT_MUTE;
        static const std::string DEFAULT_SEASON;
        static const int DEFAULT_CURRENT_CHARACTER;
        static const int DEFAULT_KEYS[Player::KEY_TOTAL];
        static const int DEFAULT_HIGHSCORES[HIGHSCORE_TOTAL];

        static bool fullscreen;
        static float volume;
        static bool mute;
        static std::string season;
        static int currentCharacter;
        static int keys[Player::KEY_TOTAL];
        static int highscores[HIGHSCORE_TOTAL];
};

#endif // INCLUDE_SAVE
