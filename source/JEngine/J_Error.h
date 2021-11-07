#ifndef INCLUDE_J_ERROR
#define INCLUDE_J_ERROR

#include <cstring>
#include <cstdlib>
#include <string>
#include <fstream>
#include <unordered_map>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

class J_Error
{
    public:

        static void initialise();

        static void log(const std::string);

        static void terminate();

    private:

        enum Type
        {
            TYPE_STD,
            TYPE_SDL,
            TYPE_IMG,
            TYPE_MIX
        };

        static const std::string ERROR_FILE;
        static const std::string ERROR_LOG;
        static int total;
        static std::unordered_map <std::string, std::string> error;

        static void messageBox(const std::string);
};

#endif // INCLUDE_J_ERROR
