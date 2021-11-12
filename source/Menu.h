#ifndef INCLUDE_MENU
#define INCLUDE_MENU

class Menu
{
    public:

        enum State
        {
            STATE_ACTIVE,
            STATE_PLAY,
            STATE_CHARACTER,
            STATE_STATS,
            STATE_CONFIG
        };

        static void initialise(J_Font*);

        static void handle();

        static void step();

        static void render();

        static void play();
        static void character();
        static void stats();
        static void config();
        static void exit();

        static int getState();

        static void terminate();

    private:

        static J_Quad background;
        static J_Image title;
        static J_Point titlePos;
        static J_Vector titleVel;
        static bool titlePlaced;
        static J_Font* font;
        static ButtonList button;
        static J_Sound tick;
        static bool halloween;
        static int state;
};



class Character
{
    public:

        enum State
        {
            STATE_ACTIVE,
            STATE_INACTIVE
        };

        static void initialise(J_Font*);

        static void handle();

        static void render();

        static void menu();

        static int getState();

        static void terminate();

    private:

        static const std::string CHARACTER_FILE;
        static J_Quad background;
        static J_Image select;
        static J_Image character;
        static J_Quad* characterSprite;
        static J_Font* font;
        static int characterTotal;
        static int currentCharacter;
        static ButtonList button;
        static J_Sound tick;
        static int unlock;
        static J_Sound glitch[4];
        static std::string* name;
        static int state;
};



class Config
{
    public:

        enum State
        {
            STATE_ACTIVE,
            STATE_REBIND,
            STATE_INACTIVE
        };

        static void initialise(J_Font*);

        static void handle();

        static void step();

        static void render();

        static void decreaseVolume();
        static void increaseVolume();
        static void toggleMute();
        static void toggleFullscreen();
        static void toggleHalloween();
        static void rebind();
        static void reset();
        static void menu();

        static int getState();

        static void terminate();

    private:

        static J_Quad background;
        static J_Font* font;
        static ButtonList button;
        static J_Sound tick;
        static int key[Player::KEY_TOTAL];
        static int state;
};



class Stat
{
    public:

        enum State
        {
            STATE_ACTIVE,
            STATE_RESETTING,
            STATE_INACTIVE
        };

        static void initialise(J_Font*);

        static void handle();

        static void render();

        static void menu();
        static void reset();
        static void no();
        static void yes();

        static int getState();

        static void terminate();

    private:

        static J_Quad background;
        static J_Font* font;
        static std::string highscores[10];
        static J_Sound resetNoise;
        static ButtonList buttonMain, buttonReset;
        static J_Sound tick;
        static int state;

        static void update();
};

#endif // INCLUDE_MENU
