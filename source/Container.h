#ifndef INCLUDE_CONTAINER
#define INCLUDE_CONTAINER

#include <ctime>
#include "Menu.h"

class Container
{
    public:

        static void initialise();

        static void handle();

        static void step();

        static void render();

        static void terminate();

    private:

        static J_Font font;
};

#endif // INCLUDE_CONTAINER
