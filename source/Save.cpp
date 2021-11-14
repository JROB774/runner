/// SAVE //////////////////////////////////////////////////////////////////////

const std::string Save::SAVE_FILE = "save.dat";

const bool Save::DEFAULT_FULLSCREEN = false;
const int Save::DEFAULT_WINDOW_WIDTH = 0;
const int Save::DEFAULT_WINDOW_HEIGHT = 0;
const float Save::DEFAULT_VOLUME = 1.0f;
const bool Save::DEFAULT_MUTE = false;
const std::string Save::DEFAULT_SEASON = "None";
const int Save::DEFAULT_KEYS[Player::KEY_TOTAL] = { 1073741906, 1073741905, 32 };
const int Save::DEFAULT_HIGHSCORES[HIGHSCORE_TOTAL] = {};
const int Save::DEFAULT_CHARACTER = 0;

bool Save::fullscreen;
float Save::volume;
bool Save::mute;
std::string Save::season;
int Save::character;
int Save::keys[Player::KEY_TOTAL];
int Save::highscores[HIGHSCORE_TOTAL];

void Save::step ()
{
    // Checks if any of the valus the save system cares about have changed this frame, and if they have it saves them out.
    bool changed = false;

    bool currFullscreen = J_Window::getFullscreen();
    float currSoundVolume = J_Mixer::getSoundVolume();
    bool currMute = J_Mixer::isMuted();
    std::string currSeason = Game::getSeason();
    int currCharacter = Player::getCharacter();
    int currKeys[Player::KEY_TOTAL] = {};
    int currHighscores[HIGHSCORE_TOTAL] = {}; // @ICNOMPLETE!

    currKeys[0] = Player::getKey(0);
    currKeys[1] = Player::getKey(1);
    currKeys[2] = Player::getKey(2);

    if(fullscreen != currFullscreen)
    {
        fullscreen = currFullscreen;
        changed = true;
    }
    if(volume != currSoundVolume)
    {
        volume = currSoundVolume;
        changed = true;
    }
    if(mute != currMute)
    {
        mute = currMute;
        changed = true;
    }
    if(season != currSeason)
    {
        season = currSeason;
        changed = true;
    }
    if(character != currCharacter)
    {
        character = currCharacter;
        changed = true;
    }
    for(int i=0; i<Player::KEY_TOTAL; ++i)
    {
        if(keys[i] != currKeys[i])
        {
            keys[i] = currKeys[i];
            changed = true;
        }
    }
    for(int i=0; i<HIGHSCORE_TOTAL; ++i)
    {
        if(highscores[i] != currHighscores[i])
        {
            highscores[i] = currHighscores[i];
            changed = true;
        }
    }

    if(changed)
    {
        DEBUG_LOG("Changed!\n");
        save();
    }
}

void Save::load ()
{
    DEBUG_LOG("Loading!\n");

    int windowWidth = DEFAULT_WINDOW_WIDTH;
    int windowHeight = DEFAULT_WINDOW_HEIGHT;

    fullscreen = DEFAULT_FULLSCREEN;
    volume = DEFAULT_VOLUME;
    mute = DEFAULT_MUTE;
    season = DEFAULT_SEASON;
    character = DEFAULT_CHARACTER;
    memcpy(keys, DEFAULT_KEYS, sizeof(keys));
    memcpy(highscores, DEFAULT_HIGHSCORES, sizeof(highscores));

    std::fstream file(FILESYS_NAME(SAVE_FILE), std::ios::in);

    // If there's no save file we just use the default values.
    if(file.is_open())
    {
        file >> fullscreen
             >> windowWidth
             >> windowHeight
             >> volume
             >> mute
             >> season
             >> character
             >> keys[0] >> keys[1] >> keys[2]
             >> highscores[0] >> highscores[1] >> highscores[2] >> highscores[3] >> highscores[4]
             >> highscores[5] >> highscores[6] >> highscores[7] >> highscores[8] >> highscores[9];
        file.close();
    }

    // We never start in fullscreen on web as it causes some problems!
    #ifdef PLATFORM_WEB
    fullscreen = false;
    #endif

    J_Window::setWidth(windowWidth); // Needs to be before fullscreen to set correctly!
    J_Window::setHeight(windowHeight); // Needs to be before fullscreen to set correctly!
    J_Window::setFullscreen(fullscreen);
    J_Mixer::setSoundVolume(volume);
    J_Mixer::setMute(mute);
    Game::setSeason(season);
    Player::setCharacter(character);
    Player::setKey(0, keys[0]);
    Player::setKey(1, keys[1]);
    Player::setKey(2, keys[2]);
    // @INCOMPLETE: Highscores...

    FILESYS_SYNC();
}

void Save::save ()
{
    DEBUG_LOG("Saving!\n");

    int windowWidth = J_Window::getWidth();
    int windowHeight = J_Window::getHeight();

    std::fstream file(FILESYS_NAME(SAVE_FILE), std::ios::out);

    if(file.is_open())
    {
        file << fullscreen << " "
             << windowWidth << " "
             << windowHeight << " "
             << volume << " "
             << mute << " "
             << season << " "
             << character << " "
             << keys[0] << " " << keys[1] << " " << keys[2] << " "
             << highscores[0] << " " << highscores[1] << " " << highscores[2] << " " << highscores[3] << " " << highscores[4] << " "
             << highscores[5] << " " << highscores[6] << " " << highscores[7] << " " << highscores[8] << " " << highscores[9];
        file.close();
    }
    else { J_Error::log("GAME_ERROR_SAVE"); }

    FILESYS_SYNC();
}

/// SAVE //////////////////////////////////////////////////////////////////////
