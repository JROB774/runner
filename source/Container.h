#ifndef INCLUDE_CONTAINER
#define INCLUDE_CONTAINER

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
