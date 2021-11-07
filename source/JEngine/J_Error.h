#ifndef INCLUDE_J_ERROR
#define INCLUDE_J_ERROR

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
