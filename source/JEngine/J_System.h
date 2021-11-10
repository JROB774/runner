#ifndef INCLUDE_J_SYSTEM
#define INCLUDE_J_SYSTEM

#ifdef BUILD_DEBUG
#define RES_DIR_ANIMATIONS  "../../assets/Animations/"
#define RES_DIR_BACKGROUNDS "../../assets/Backgrounds/"
#define RES_DIR_DATA        "../../assets/Data/"
#define RES_DIR_FONTS       "../../assets/Fonts/"
#define RES_DIR_IMAGES      "../../assets/Images/"
#define RES_DIR_SOUNDS      "../../assets/Sounds/"
#else
#define RES_DIR_ANIMATIONS  "Resources/Animations/"
#define RES_DIR_BACKGROUNDS "Resources/Backgrounds/"
#define RES_DIR_DATA        "Resources/Data/"
#define RES_DIR_FONTS       "Resources/Fonts/"
#define RES_DIR_IMAGES      "Resources/Images/"
#define RES_DIR_SOUNDS      "Resources/Sounds/"
#endif

class J_System
{
    public:

        static const int FRAME_CAP;

        enum State
        {
            STATE_INACTIVE,
            STATE_RESETTING,
            STATE_ACTIVE
        };

        static void initialise();

        static int pollEvent();
        static void handle();

        static void stepBegin();
        static void stepEnd();

        static void start();
        static void restart();
        static void stop();

        static SDL_Event getEvent();
        static J_Timer* getFpsTimer();
        static J_Timer* getCapTimer();
        static int getFps();
        static bool isDebug();
        static int getState();

        static void terminate();

    private:

        static const std::string SYSTEM_FILE;
        static SDL_Event event;
        static J_Timer fpsTimer, capTimer;
        static int fps, tpf;
        static int frameCount;
        static double averageFps;
        static bool debug;
        static int state;
};

#endif // INCLUDE_J_SYSTEM
