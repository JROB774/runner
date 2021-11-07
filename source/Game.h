#ifndef INCLUDE_GAME
#define INCLUDE_GAME

class Game
{
    public:

        enum State
        {
            STATE_ACTIVE,
            STATE_PAUSED,
            STATE_INACTIVE,
            STATE_TERMINATE
        };

        static void initialise(J_Font*);

        static void handle();

        static void step();

        static void render();

        static int getScore();
        static int getState();

        static void terminate();

    private:

        static const std::string HALLOWEEN_FILE;
        static J_Background front;
        static J_Background back;
        static J_Collider ground;
        static J_Font* font;
        static J_Font bigFont;
        static J_Quad bar;
        static int score, bestScore;
        static int inputBuffer;
        static bool halloween;
        static int state;
};

#endif // INCLUDE_GAME
