/// MENU //////////////////////////////////////////////////////////////////////

const std::string Menu::HALLOWEEN_FILE = "Resources\\Data\\Halloween.dat";
J_Quad Menu::background;
J_Image Menu::title;
J_Point Menu::titlePos;
J_Vector Menu::titleVel;
bool Menu::titlePlaced = false;
J_Font* Menu::font = nullptr;
ButtonList Menu::button;
J_Sound Menu::tick;
bool Menu::halloween = false;
int Menu::state = -1;



void Menu::initialise (J_Font* a_font)
{
    std::ifstream file(HALLOWEEN_FILE);

    if (file.is_open())
    {
        file >> halloween;
        file.close();
    }
    else { J_Error::log("GAME_ERROR_LOAD_HALLOWEEN"); }

    background.quad = { 0, 0, J_Window::getScreenWidth(), J_Window::getScreenHeight() };
    background.colour = { 255, 255, 255, 255, SDL_BLENDMODE_BLEND };

    title.create((halloween) ? "Halloween\\Title" : "Title");
    titlePos.point = { -J_Window::getScreenWidth(), 0 };
    titleVel = { 6, 0 };
    titlePlaced = false;

    font = a_font;

    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 4) / 2), 88, "Play", font, &play);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 9) / 2), 96, "Character", font, &character);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 10) / 2), 104, "Highscores", font, &stats);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 6) / 2), 112, "Config", font, &config);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 4) / 2), 120, "Exit", font, &exit);

    tick.create("Tick", 0);

    state = STATE_ACTIVE;
}



void Menu::handle ()
{
    if (state == STATE_ACTIVE)
    {
        if (!titlePlaced)
        {
            if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
            {
                titlePlaced = true;
                titlePos.point.x = 0;
            }
        }
        else
        {
            if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
            {
                switch (J_Input::getInput().key.keysym.sym)
                {
                    case (SDLK_UP): { tick.play(0); button.back(); break; }
                    case (SDLK_DOWN): { tick.play(0); button.forward(); break; }

                    case (SDLK_SPACE): { button.press(); break; }
                    case (SDLK_RETURN): { button.press(); break; }
                }
            }
        }
    }
}



void Menu::step ()
{
    if (state == STATE_ACTIVE)
    {
        if (titlePos.point.x < 0)
        {
            titlePos.point.x += (int)titleVel.x;

            if (titlePos.point.x >= 0)
            {
                titlePos.point.x = 0;
                titlePlaced = true;
            }
        }
    }
}



void Menu::render ()
{
    if (state == STATE_ACTIVE)
    {
        J_Renderer::drawQuadFilled(background);
        title.render(titlePos.point.x, titlePos.point.y, nullptr);

        if (titlePlaced) { button.render(); }
    }
}



void Menu::play ()
{
    state = STATE_PLAY;
}

void Menu::character ()
{
    state = STATE_CHARACTER;
}

void Menu::stats ()
{
    state = STATE_STATS;
}

void Menu::config ()
{
    state = STATE_CONFIG;
}

void Menu::exit ()
{
    J_System::stop();
}



int Menu::getState ()
{
    return state;
}



void Menu::terminate ()
{
    title.destroy();
    titlePos.point = { 0, 0 };
    titleVel = { 0, 0 };
    titlePlaced = false;

    font = nullptr;

    button.destroy();

    tick.destroy();
}

/// MENU //////////////////////////////////////////////////////////////////////



/// CHARACTER /////////////////////////////////////////////////////////////////

const std::string Character::CHARACTER_FILE = "Resources\\Data\\Characters.dat";
const std::string Character::SAVE_FILE = "Resources\\Data\\Save.dat";
J_Quad Character::background;
J_Image Character::select;
J_Image Character::character;
J_Quad* Character::characterSprite = nullptr;
J_Font* Character::font = nullptr;
int Character::characterTotal = 0;
int Character::currentCharacter = -1;
ButtonList Character::button;
J_Sound Character::tick;
int Character::unlock = 0;
J_Sound Character::glitch[4];
std::string* Character::name = nullptr;
int Character::state = -1;



void Character::initialise (J_Font* a_font)
{
    background.quad = { 0, 0, J_Window::getScreenWidth(), J_Window::getScreenHeight() };
    background.colour = { 255, 255, 255, 255, SDL_BLENDMODE_BLEND };

    select.create("Select");
    character.create("Character");

    characterTotal = (character.getWidth() / 24);

    characterSprite = new(std::nothrow) J_Quad[characterTotal];
    if (characterSprite == nullptr) { J_Error::log("GAME_ERROR_CHARACTER_INIT"); }
    for (int i = 0, ix = 0; i < characterTotal; ++i, ix += 24) { characterSprite[i].quad = { ix, 0, 24, 24 }; }

    font = a_font;

    std::ifstream file(SAVE_FILE);
    std::string rawData = "\0";

    if (file.is_open())
    {
        std::getline(file, rawData);
        file.close();
    }
    else { J_Error::log("GAME_ERROR_SAVE_LOAD"); }

    currentCharacter = atoi(rawData.c_str());

    button.create(0, 0, "\0", nullptr, &menu);

    tick.create("Tick", 0);

    unlock = 0;
    for (int i = 0; i < 4; ++i) { glitch[i].create("Glitch" + std::to_string(i), i + 1); }

    name = new(std::nothrow) std::string[characterTotal];
    if (name == nullptr) { J_Error::log("GAME_ERROR_CHARACTER_INIT"); }

    file.open(CHARACTER_FILE);

    if (file.is_open())
    {
        for (int i = 0; i < characterTotal; ++i) { std::getline(file, name[i]); }
        file.close();
    }
    else { J_Error::log("GAME_ERROR_CHARACTER_LOAD"); }

    state = STATE_ACTIVE;
}



void Character::handle ()
{
    if (state == STATE_ACTIVE)
    {
        if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
        {
            switch (J_Input::getInput().key.keysym.sym)
            {
                case (SDLK_LEFT):
                {
                    tick.play(0);

                    if (unlock < 40)
                    {
                        ++unlock;

                        --currentCharacter;
                        if (currentCharacter < 0) { currentCharacter = (characterTotal - 2); }
                    }

                    break;
                }

                case (SDLK_RIGHT):
                {
                    tick.play(0);

                    if (unlock < 40)
                    {
                        ++unlock;

                        ++currentCharacter;
                        if (currentCharacter >= (characterTotal - 1)) { currentCharacter = 0; }
                    }

                    break;
                }

                case (SDLK_SPACE): { button.press(); break; }
                case (SDLK_RETURN): { button.press(); break; }
            }
        }
    }

    if (unlock == 40)
    {
        for (int i = 0; i < 4; ++i) { glitch[i].play(0); }
        currentCharacter = characterTotal - 1;
        ++unlock;
    }
}



void Character::render ()
{
    if (state == STATE_ACTIVE)
    {
        J_Renderer::drawQuadFilled(background);

        J_Colour grey = { 83, 83, 83, 255, SDL_BLENDMODE_BLEND };
        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 16) / 2), 0,
                     "Select Character", grey);

        character.render((J_Window::getScreenWidth() / 2) - 12, (J_Window::getScreenHeight() / 2) - 12, &characterSprite[currentCharacter]);
        select.render((J_Window::getScreenWidth() / 2) - (select.getWidth() / 2), (J_Window::getScreenHeight() / 2) - (select.getHeight() / 2), nullptr);

        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)name[currentCharacter].length()) / 2),
                     (J_Window::getScreenHeight() / 2) + 16, name[currentCharacter], grey);
    }
}



void Character::menu ()
{
    std::ofstream file(SAVE_FILE);

    if (file.is_open())
    {
        std::string data = std::to_string(currentCharacter);

        file << data;
        file.close();
    }
    else { J_Error::log("GAME_ERROR_SAVE_SAVE"); }

    state = STATE_INACTIVE;
}



int Character::getState ()
{
    return state;
}



void Character::terminate ()
{
    select.destroy();
    character.destroy();;

    font = nullptr;

    delete[] characterSprite;
    characterSprite = nullptr;

    characterTotal = 0;
    currentCharacter = -1;

    button.destroy();

    tick.destroy();

    unlock = 0;
    for (int i = 0; i < 4; ++i) { glitch[i].destroy(); }

    delete[] name;
    name = nullptr;

    state = -1;
}

/// CHARACTER /////////////////////////////////////////////////////////////////



/// CONFIG ////////////////////////////////////////////////////////////////////

const std::string Config::KEY_FILE = "Resources\\Data\\Keys.dat";
const std::string Config::HALLOWEEN_FILE = "Resources\\Data\\Halloween.dat";
const std::string Config::CONFIG_FILE = "Resources\\Data\\Config.dat";
J_Quad Config::background;
J_Font* Config::font = nullptr;
ButtonList Config::button;
J_Sound Config::tick;
int Config::key[Player::KEY_TOTAL];
int Config::state = -1;



void Config::initialise (J_Font* a_font)
{
    background.quad = { 0, 0, J_Window::getScreenWidth(), J_Window::getScreenHeight() };
    background.colour = { 255, 255, 255, 255, SDL_BLENDMODE_BLEND };

    font = a_font;

    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 15) / 2), 44, "Decrease Volume",   font, &decreaseVolume);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 15) / 2), 52, "Increase Volume",   font, &increaseVolume);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 11) / 2), 60, "Toggle Mute",       font, &toggleMute);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 17) / 2), 68, "Toggle Fullscreen", font, &toggleFullscreen);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 16) / 2), 76, "Toggle Halloween",  font, &toggleHalloween);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 11) / 2), 84, "Rebind Keys",       font, &rebind);
    button.create((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() *  4) / 2), 92, "Back",              font, &menu);

    tick.create("Tick", 0);

    for (int i = 0; i < Player::KEY_TOTAL; ++i) { key[i] = -1; }

    state = STATE_ACTIVE;
}



void Config::handle ()
{
    if (state == STATE_ACTIVE)
    {
        if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
        {
            switch (J_Input::getInput().key.keysym.sym)
            {
                case (SDLK_UP): { tick.play(0); button.back(); break; }
                case (SDLK_DOWN): { tick.play(0); button.forward(); break; }

                case (SDLK_SPACE): { button.press(); break; }
                case (SDLK_RETURN): { button.press(); break; }
            }
        }
    }
    else if (state == STATE_REBIND)
    {
        if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
        {
            if      (key[Player::KEY_JUMP]  == -1) { key[Player::KEY_JUMP]  = J_Input::getInput().key.keysym.sym; }
            else if (key[Player::KEY_SLIDE] == -1) { key[Player::KEY_SLIDE] = J_Input::getInput().key.keysym.sym; }
            else if (key[Player::KEY_PAUSE] == -1)
            {
                key[Player::KEY_PAUSE] = J_Input::getInput().key.keysym.sym;

                std::ofstream file(KEY_FILE);

                if (file.is_open())
                {
                    file << key[Player::KEY_JUMP] << " " << key[Player::KEY_SLIDE] << " " << key[Player::KEY_PAUSE];
                    file.close();
                }
                else { J_Error::log("GAME_ERROR_KEY_SAVE"); }

                state = STATE_ACTIVE;
            }
        }
    }
}



void Config::render ()
{
    if (state == STATE_ACTIVE)
    {
         J_Renderer::drawQuadFilled(background);

        J_Colour grey = { 83, 83, 83, 255, SDL_BLENDMODE_BLEND };
        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 6) / 2), 0,
                     "Config", grey);

        button.render();
    }
    else if (state == STATE_REBIND)
    {
        J_Renderer::drawQuadFilled(background);

        J_Colour grey = { 83, 83, 83, 255, SDL_BLENDMODE_BLEND };
        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 6) / 2), 0,
                     "Config", grey);

        std::string piece = "\0";

        if      (key[Player::KEY_JUMP]  == -1) { piece = "Jump";  }
        else if (key[Player::KEY_SLIDE] == -1) { piece = "Slide"; }
        else if (key[Player::KEY_PAUSE] == -1) { piece = "Pause"; }

        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (18 + (int)piece.length())) / 2),
                     (J_Window::getScreenHeight() / 2) - (font->getCharHeight() / 2),
                     "Enter the new " + piece + " key", grey);
    }
}



void Config::decreaseVolume ()
{
    J_Mixer::setSoundVolume(J_Mixer::getSoundVolume() - J_Mixer::getVolumePiece());
}

void Config::increaseVolume ()
{
    J_Mixer::setSoundVolume(J_Mixer::getSoundVolume() + J_Mixer::getVolumePiece());
}

void Config::toggleMute ()
{
    J_Mixer::toggleMute();
}

void Config::toggleFullscreen ()
{
    J_Window::toggleFullscreen();
}

void Config::toggleHalloween ()
{
    bool halloween = false;
    std::fstream file(HALLOWEEN_FILE, std::ios::in);

    if (file.is_open())
    {
        file >> halloween;
        file.close();
    }
    else { J_Error::log("GAME_ERROR_LOAD_HALLOWEEN"); }

    halloween = !halloween;

    file.open(HALLOWEEN_FILE, std::ios::out);

    if (file.is_open())
    {
        file << halloween;
        file.close();
    }
    else { J_Error::log("GAME_ERROR_SAVE_HALLOWEEN"); }
}

void Config::rebind ()
{
    for (int i = 0; i < Player::KEY_TOTAL; ++i) { key[i] = -1; }
    state = STATE_REBIND;
}

void Config::menu ()
{
    std::fstream file(CONFIG_FILE, std::ios::in);
    std::string rawData = "\0";

    if (file.is_open())
    {
        std::getline(file, rawData);
        file.close();
    }
    else { J_Error::log("GAME_ERROR_LOAD_CONFIG"); }

    std::istringstream stream;
    stream.str(rawData);

    bool fullscreen = false;
    int scale = 0; // This is now unused! (v1.3.0)
    float volume = 0.0;
    bool mute = false;

    stream >> fullscreen >> scale >> volume >> mute;
    stream.str("\0");

    fullscreen = J_Window::getFullscreen();
    volume = J_Mixer::getSoundVolume();
    mute = J_Mixer::isMuted();

    file.open(CONFIG_FILE, std::ios::out);

    if (file.is_open())
    {
        file << fullscreen << " " << scale << " " << volume << " " << mute;
        file.close();
    }
    else { J_Error::log("GAME_ERROR_SAVE_CONFIG"); }

    state = STATE_INACTIVE;
}



int Config::getState ()
{
    return state;
}



void Config::terminate ()
{
    font = nullptr;

    button.destroy();

    tick.destroy();

    for (int i = 0; i < Player::KEY_TOTAL; ++i) { key[i] = -1; }

    state = -1;
}

/// CONFIG ////////////////////////////////////////////////////////////////////



/// STAT //////////////////////////////////////////////////////////////////////

J_Quad Stat::background;
J_Font* Stat::font = nullptr;
std::string Stat::highscores[10];
J_Sound Stat::resetNoise;
ButtonList Stat::buttonMain, Stat::buttonReset;
J_Sound Stat::tick;
int Stat::state = -1;



void Stat::initialise (J_Font* a_font)
{
    background.quad = { 0, 0, J_Window::getScreenWidth(), J_Window::getScreenHeight() };
    background.colour = { 255, 255, 255, 255, SDL_BLENDMODE_BLEND };

    font = a_font;

    resetNoise.create("Dead", 0);

    update();

    buttonMain.create(0, (J_Window::getScreenHeight() - 14), "Back", font, &menu);
    buttonMain.create((J_Window::getScreenWidth() - ((font->getCharWidth() * 7))), (J_Window::getScreenHeight() - 14), "Reset", font, &reset);

    buttonReset.create(0, (J_Window::getScreenHeight() - 14), "No", font, &no);
    buttonReset.create((J_Window::getScreenWidth() - ((font->getCharWidth() * 5))), (J_Window::getScreenHeight() - 14), "Yes", font, &yes);

    tick.create("Tick", 1);

    state = STATE_ACTIVE;
}



void Stat::handle ()
{
    if (state == STATE_ACTIVE)
    {
        if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
        {
            switch (J_Input::getInput().key.keysym.sym)
            {
                case (SDLK_LEFT): { tick.play(0); buttonMain.back(); break; }
                case (SDLK_RIGHT): { tick.play(0); buttonMain.forward(); break; }

                case (SDLK_SPACE): { buttonMain.press(); break; }
                case (SDLK_RETURN): { buttonMain.press(); break; }
            }
        }
    }
    else if (state == STATE_RESETTING)
    {
        if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
        {
            switch (J_Input::getInput().key.keysym.sym)
            {
                case (SDLK_LEFT): { tick.play(0); buttonReset.back(); break; }
                case (SDLK_RIGHT): { tick.play(0); buttonReset.forward(); break; }

                case (SDLK_SPACE): { buttonReset.press(); break; }
                case (SDLK_RETURN): { buttonReset.press(); break; }
            }
        }
    }
}



void Stat::render ()
{
    if (state == STATE_ACTIVE)
    {
        J_Renderer::drawQuadFilled(background);

        J_Colour grey = { 83, 83, 83, 255, SDL_BLENDMODE_BLEND };

        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 10) / 2), 0,
                     "Highscores", grey);

        for (int i = 0, iy = 32; i < 10; ++i, iy += font->getCharHeight())
        {
            font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)highscores[i].length()) / 2),
                         iy, highscores[i], grey);
        }

        buttonMain.render();
    }
    else if (state == STATE_RESETTING)
    {
        J_Renderer::drawQuadFilled(background);

        J_Colour grey = { 83, 83, 83, 255, SDL_BLENDMODE_BLEND };

        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 10) / 2), 0,
                     "Highscores", grey);

        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 32) / 2),
                     (J_Window::getScreenHeight() / 2) - (font->getCharHeight() / 2),
                     "Are you sure you want to reset?", grey);

        buttonReset.render();
    }
}



void Stat::menu ()
{
    state = STATE_INACTIVE;
}

void Stat::reset ()
{
    state = STATE_RESETTING;
}

void Stat::no ()
{
    state = STATE_ACTIVE;
}

void Stat::yes ()
{
    Highscore::reset();
    update();
    resetNoise.play(0);

    state = STATE_ACTIVE;
}



int Stat::getState ()
{
    return state;
}



void Stat::terminate ()
{
    font = nullptr;

    for (int i = 0; i < 10; ++i) { highscores[i] = "\0"; }

    resetNoise.destroy();

    buttonMain.destroy();
    buttonReset.destroy();

    tick.destroy();

    state = -1;
}



void Stat::update ()
{
    std::string rawData = Highscore::load();
    char* data = &rawData[0];

    for (unsigned int i = 0; i < rawData.length(); ++i) { if (rawData[i] == ' ') { rawData[i] = '\n'; } }
    highscores[0] = strtok(data, "\n");
    for (int i = 1; i < 10; ++i) { highscores[i] = strtok(nullptr, "\n"); }

    data = nullptr;
}

/// STAT //////////////////////////////////////////////////////////////////////
