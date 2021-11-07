#ifndef INCLUDE_GAME_OVER
#define INCLUDE_GAME_OVER

class GameOver
{
    public:

        enum State
        {
            STATE_ACTIVE,
            STATE_RESTART,
            STATE_MENU
        };

        static void initialise(J_Font*, const int);

        static void handle();

        static void step();

        static void render();

        static void restart();
        static void menu();

        static int getState();

        static void terminate();

    private:

        enum Medal
        {
            MEDAL_EMPTY,
            MEDAL_F,
            MEDAL_E,
            MEDAL_D,
            MEDAL_C,
            MEDAL_B,
            MEDAL_A,
            MEDAL_TOTAL
        };

        static const std::string HALLOWEEN_FILE;
        static J_Quad background;
        static J_Image medal;
        static J_Quad sprite[MEDAL_TOTAL];
        static J_Font* font;
        static J_Sound scoreS, medalS, bestS;
        static int score, finalScore;
        static int grade;
        static int startCounter, medalCounter;
        static bool newBest;
        static ButtonList button;
        static J_Sound tick;
        static bool halloween;
        static int state;
};

#endif // INCLUDE_GAME_OVER
