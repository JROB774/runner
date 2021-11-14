#ifndef INCLUDE_HIGHSCORE
#define INCLUDE_HIGHSCORE

static const int HIGHSCORE_TOTAL = 10;

class Highscore
{
    public:

        static void setScores(const int*);
        static int* getScores();

        static bool addScore(const int);
        static void reset();

    private:

        static int highscores[HIGHSCORE_TOTAL];
};

#endif // INCLUDE_HIGHSCORE
