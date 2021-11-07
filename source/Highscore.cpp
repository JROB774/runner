/// HIGHSCORE /////////////////////////////////////////////////////////////////

const int Highscore::TOTAL = 10;
const std::string Highscore::FILE = "Resources\\Data\\Highscores.dat";



int Highscore::save (const int a_score)
{
    std::string rawData = load();
    char* data = &rawData[0];

    int highscore[TOTAL];

    highscore[0] = atoi(strtok(data, " "));
    for (int i = 1; i < TOTAL; ++i) { highscore[i] = atoi(strtok(nullptr, " ")); }

    if (a_score == -1) { return highscore[0]; }

    int position = -1;

    for (int i = 9; i >= 0; --i)
    {
        if (a_score > highscore[i]) { position = i; continue; }
        else { break; }
    }

    if (position > -1)
    {
        for (int i = (TOTAL - 1); i >= position; --i) { highscore[i] = highscore[i - 1]; }
        highscore[position] = a_score;

        std::string toSave = "\0";
        for (int i = 0; i < TOTAL; ++i) { toSave += (std::to_string(highscore[i]) + "\n"); }

        std::ofstream file(FILE);

        if (file.is_open())
        {
            file << toSave;
            file.close();
        }
        else { J_Error::log("GAME_ERROR_SAVE_HIGHSCORE"); }

        return (position == 0) ? 1 : 0;
    }

    return -1;
}

std::string Highscore::load ()
{
    std::ifstream file(FILE);
    std::string data = "\0";

    if (file.is_open())
    {
        for (int i = 0; i < TOTAL; ++i)
        {
            std::string rawData = "\0";
            std::getline(file, rawData);

            data += (rawData + " ");
        }

        file.close();
    }
    else { J_Error::log("GAME_ERROR_LOAD_HIGHSCORE"); }

    return data;
}



void Highscore::reset ()
{
    std::ofstream file(FILE);

    if (file.is_open())
    {
        for (int i = 0; i < TOTAL; ++i) { file << "0\n"; }
        file.close();
    }
    else { J_Error::log("GAME_ERROR_RESET_HIGHSCORE"); }
}

/// HIGHSCORE /////////////////////////////////////////////////////////////////
