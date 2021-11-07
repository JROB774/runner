#ifndef INCLUDE_J_SYSTEM
#define INCLUDE_J_SYSTEM

#include "J_Time.h"
#include "J_Window.h"
#include "J_Collision.h"
#include "J_Audio.h"
#include "J_Input.h"
#include "J_Math.h"

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
        static const std::string CONFIG_FILE;
        static SDL_Event event;
        static J_Timer fpsTimer, capTimer;
        static int fps, tpf;
        static int frameCount;
        static double averageFps;
        static bool debug;
        static int state;
};

#endif // INCLUDE_J_SYSTEM
