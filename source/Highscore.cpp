/// HIGHSCORE /////////////////////////////////////////////////////////////////

int Highscore::highscores[HIGHSCORE_TOTAL];

static int scoreCompare (const void* a, const void* b)
{
    int* aa = (int*)a;
    int* bb = (int*)b;

    if (*aa > *bb) return -1;
    if (*aa < *bb) return  1;

    return 0;
}

void Highscore::setScores (const int* a_scores)
{
    memcpy(highscores, a_scores, sizeof(highscores));
}

int* Highscore::getScores ()
{
    return highscores;
}

bool Highscore::addScore (const int a_score)
{
    bool newBest = (a_score > highscores[0]);
    if (a_score > highscores[9])
    {
        highscores[9] = a_score;
        qsort(highscores, HIGHSCORE_TOTAL, sizeof(int), scoreCompare);
    }
    return newBest;
}

void Highscore::reset ()
{
    memset(highscores, 0, sizeof(highscores));
}

/// HIGHSCORE /////////////////////////////////////////////////////////////////
