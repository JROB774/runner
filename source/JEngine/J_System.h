#ifndef INCLUDE_J_SYSTEM
#define INCLUDE_J_SYSTEM

#ifdef BUILD_DEBUG
#define DEBUG_LOG(...) printf(__VA_ARGS__)
#else
#define DEBUG_LOG(...)
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
        static int getFps();
        static bool isDebug();
        static int getState();

        static void terminate();

    private:

        static const std::string SYSTEM_FILE;
        static SDL_Event event;
        static int fps, tpf;
        static int frameCount;
        static double averageFps;
        static bool debug;
        static int state;
};

#endif // INCLUDE_J_SYSTEM
