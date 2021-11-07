#ifndef INCLUDE_HIGHSCORE
#define INCLUDE_HIGHSCORE

class Highscore
{
    public:

        static const int TOTAL;

        static int save(const int);
        static std::string load();

        static void reset();

    private:

        static const std::string FILE;
};

#endif // INCLUDE_HIGHSCORE
