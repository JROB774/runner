#ifndef INCLUDE_J_INPUT
#define INCLUDE_J_INPUT

#include <vector>
#include "J_Error.h"

class J_Input
{
    public:

        static const int TOTAL_KEYS;
        static const int TOTAL_BUTTONS;

        static void initialise(SDL_Event*);

        static bool keyPress(const int);
        static bool keyRelease(const int);

        static bool mousePress(const int);
        static bool mouseRelease(const int);
        static void mouseMotion(int*, int*);

        static SDL_Event& getInput();

        static void terminate();

    private:

        static bool* key;
        static bool* button;
        static SDL_Event* input;
};

#endif // INCLUDE_J_INPUT
