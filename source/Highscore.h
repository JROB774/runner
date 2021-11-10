#ifndef INCLUDE_HIGHSCORE
#define INCLUDE_HIGHSCORE

static const int HIGHSCORE_TOTAL = 10;

class Highscore
{
    public:

        static int save(const int);
        static std::string load();
        static void reset();
};

#endif // INCLUDE_HIGHSCORE
