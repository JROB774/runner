/// MENU //////////////////////////////////////////////////////////////////////

J_Quad Menu::background;
J_Image Menu::title;
J_Point Menu::titlePos;
J_Vector Menu::titleVel;
bool Menu::titlePlaced = false;
J_Font* Menu::font = nullptr;
ButtonList Menu::button;
J_Sound Menu::tick;
int Menu::state = -1;



void Menu::initialise (J_Font* a_font)
{
    background.quad = { 0, 0, J_Window::getScreenWidth(), J_Window::getScreenHeight() };
    background.colour = { 255, 255, 255, 255, SDL_BLENDMODE_BLEND };

    title.create(Game::getSeasonPath() + "Title");
    titlePos.point = { -J_Window::getScreenWidth(), 0 };
    titleVel = { 6, 0 };
    titlePlaced = false;

    font = a_font;

    button.create(Button::TYPE_PRESS, (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() *  4) / 2) - 3,  88, "Play",       font, &play);
    button.create(Button::TYPE_PRESS, (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() *  9) / 2) - 3,  96, "Character",  font, &character);
    button.create(Button::TYPE_PRESS, (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 10) / 2) - 3, 104, "Highscores", font, &stats);
    button.create(Button::TYPE_PRESS, (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() *  6) / 2) - 3, 112, "Config",     font, &config);
    #ifndef PLATFORM_WEB // We don't need or want an exit button on web builds!
    button.create(Button::TYPE_PRESS, (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() *  4) / 2) - 3, 120, "Exit",       font, &exit);
    #endif

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



void Menu::play (Button* a_button, const int a_interaction)
{
    state = STATE_PLAY;
}

void Menu::character (Button* a_button, const int a_interaction)
{
    state = STATE_CHARACTER;
}

void Menu::stats (Button* a_button, const int a_interaction)
{
    state = STATE_STATS;
}

void Menu::config (Button* a_button, const int a_interaction)
{
    state = STATE_CONFIG;
}

void Menu::exit (Button* a_button, const int a_interaction)
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

const std::string Character::CHARACTER_FILE = RES_DIR_DATA "Characters.dat";
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

    currentCharacter = Player::getCharacter();

    button.create(Button::TYPE_PRESS, 0, 0, "\0", nullptr, &menu);

    tick.create("Tick", 0);

    unlock = 0;
    for (int i = 0; i < 4; ++i) { glitch[i].create("Glitch" + std::to_string(i), i + 1); }

    name = new(std::nothrow) std::string[characterTotal];
    if (name == nullptr) { J_Error::log("GAME_ERROR_CHARACTER_INIT"); }

    std::ifstream file(CHARACTER_FILE, std::ios::in);
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



void Character::menu (Button* a_button, const int a_interaction)
{
    Player::setCharacter(currentCharacter);
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

    int volumeTengths = (int)roundf(((J_Mixer::getSoundVolume() * 100.0f) / 10.0f));

    std::string volumeText = "Volume: " + std::to_string(volumeTengths * 10);
    std::string muteText   = J_Mixer::isMuted() ? "Unmute Sound" : "Mute Sound";
    std::string windowText = J_Window::getFullscreen() ? "Go Windowed" : "Go Fullscreen";
    std::string seasonText = "Season: " + Game::getSeason();
    std::string rebindText = "Rebind Keys";
    std::string resetText  = "Reset Config";
    std::string backText   = "Back";

    #ifdef PLATFORM_WEB
    button.create(Button::TYPE_SLIDER, (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)volumeText.length()) / 2) - 3, 44, volumeText, font, &setVolume);
    button.create(Button::TYPE_PRESS,  (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)muteText.length())   / 2) - 3, 52, muteText,   font, &toggleMute);
    button.create(Button::TYPE_PRESS,  9999,                                                                                           60, windowText, font, &toggleFullscreen);
    button.create(Button::TYPE_SLIDER, (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)seasonText.length()) / 2) - 3, 60, seasonText, font, &setSeason);
    button.create(Button::TYPE_PRESS,  (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)rebindText.length()) / 2) - 3, 68, rebindText, font, &rebind);
    button.create(Button::TYPE_PRESS,  (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)resetText.length())  / 2) - 3, 76, resetText,  font, &reset);
    button.create(Button::TYPE_PRESS,  (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)backText.length())   / 2) - 3, 84, backText,   font, &menu);
    #else
    button.create(Button::TYPE_SLIDER, (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)volumeText.length()) / 2) - 3, 44, volumeText, font, &setVolume);
    button.create(Button::TYPE_PRESS,  (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)muteText.length())   / 2) - 3, 52, muteText,   font, &toggleMute);
    button.create(Button::TYPE_PRESS,  (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)windowText.length()) / 2) - 3, 60, windowText, font, &toggleFullscreen);
    button.create(Button::TYPE_SLIDER, (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)seasonText.length()) / 2) - 3, 68, seasonText, font, &setSeason);
    button.create(Button::TYPE_PRESS,  (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)rebindText.length()) / 2) - 3, 76, rebindText, font, &rebind);
    button.create(Button::TYPE_PRESS,  (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)resetText.length())  / 2) - 3, 84, resetText,  font, &reset);
    button.create(Button::TYPE_PRESS,  (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)backText.length())   / 2) - 3, 92, backText,   font, &menu);
    #endif

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

                case (SDLK_LEFT): { button.decrement(); break; }
                case (SDLK_RIGHT): { button.increment(); break; }

                case (SDLK_SPACE): { button.press(); break; }
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

                Player::setKey(Player::KEY_JUMP,  key[Player::KEY_JUMP]);
                Player::setKey(Player::KEY_SLIDE, key[Player::KEY_SLIDE]);
                Player::setKey(Player::KEY_PAUSE, key[Player::KEY_PAUSE]);

                state = STATE_ACTIVE;
            }
        }
    }
}



void Config::render ()
{
    if (state == STATE_ACTIVE)
    {
        // This can update from external places so we need to update it regularly to stop it from falling out of date.
        std::string windowText = J_Window::getFullscreen() ? "Go Windowed" : "Go Fullscreen";
        button.getButton(2)->updatePosition((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)windowText.length()) / 2) - 3, 60);
        button.getButton(2)->updateText(windowText);

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



void Config::setVolume (Button* a_button, const int a_interaction)
{
    if(a_interaction < 0) { J_Mixer::setSoundVolume(J_Mixer::getSoundVolume() - 0.1f); }
    if(a_interaction > 0) { J_Mixer::setSoundVolume(J_Mixer::getSoundVolume() + 0.1f); }

    int volumeTengths = (int)roundf(((J_Mixer::getSoundVolume() * 100.0f) / 10.0f));
    std::string volumeText = "Volume: " + std::to_string(volumeTengths * 10);

    a_button->updatePosition((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)volumeText.length()) / 2) - 3, 44);
    a_button->updateText(volumeText);
}

void Config::toggleMute (Button* a_button, const int a_interaction)
{
    J_Mixer::toggleMute();

    std::string muteText = J_Mixer::isMuted() ? "Unmute Sound" : "Mute Sound";

    a_button->updatePosition((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)muteText.length()) / 2) - 3, 52);
    a_button->updateText(muteText);
}

void Config::toggleFullscreen (Button* a_button, const int a_interaction)
{
    J_Window::toggleFullscreen();

    std::string windowText = J_Window::getFullscreen() ? "Go Windowed" : "Go Fullscreen";

    a_button->updatePosition((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)windowText.length()) / 2) - 3, 60);
    a_button->updateText(windowText);
}

void Config::setSeason (Button* a_button, const int a_interaction)
{
    static const std::string seasons[] = { "None", "Halloween" };

    int current = 0;
    for (int i = 0; i < 2; ++i)
    {
        if (Game::getSeason() == seasons[i])
        {
            current = i;
            break;
        }
    }

    current += a_interaction;

    if (current < 0) { current = 1; }
    if (current > 1) { current = 0; }

    Game::setSeason(seasons[current]);

    std::string seasonText = "Season: " + Game::getSeason();

    a_button->updatePosition((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)seasonText.length()) / 2) - 3, 68);
    a_button->updateText(seasonText);
}

void Config::rebind (Button* a_button, const int a_interaction)
{
    for (int i = 0; i < Player::KEY_TOTAL; ++i) { key[i] = -1; }
    state = STATE_REBIND;
}

void Config::reset (Button* a_button, const int a_interaction)
{
    J_Mixer::setSoundVolume(Save::DEFAULT_VOLUME);
    J_Mixer::setMute(Save::DEFAULT_MUTE);
    J_Window::setFullscreen(Save::DEFAULT_FULLSCREEN);
    J_Window::setWidth(Save::DEFAULT_WINDOW_WIDTH);
    J_Window::setHeight(Save::DEFAULT_WINDOW_HEIGHT);
    Game::setSeason(Save::DEFAULT_SEASON);

    int volumeTengths = (int)roundf(((J_Mixer::getSoundVolume() * 100.0f) / 10.0f));

    std::string volumeText = "Volume: " + std::to_string(volumeTengths * 10);
    std::string muteText   = J_Mixer::isMuted() ? "Unmute Sound" : "Mute Sound";
    std::string windowText = J_Window::getFullscreen() ? "Go Windowed" : "Go Fullscreen";
    std::string seasonText = "Season: " + Game::getSeason();

    button.getButton(0)->updatePosition((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)volumeText.length()) / 2) - 3, 44);
    button.getButton(1)->updatePosition((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)muteText.length())   / 2) - 3, 52);
    button.getButton(2)->updatePosition((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)windowText.length()) / 2) - 3, 60);
    button.getButton(3)->updatePosition((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)seasonText.length()) / 2) - 3, 68);

    button.getButton(0)->updateText(volumeText);
    button.getButton(1)->updateText(muteText);
    button.getButton(2)->updateText(windowText);
    button.getButton(3)->updateText(seasonText);
}

void Config::menu (Button* a_button, const int a_interaction)
{
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

    buttonMain.create(Button::TYPE_PRESS, 0, (J_Window::getScreenHeight() - 14), "Back", font, &menu);
    buttonMain.create(Button::TYPE_PRESS, (J_Window::getScreenWidth() - ((font->getCharWidth() * 7))), (J_Window::getScreenHeight() - 14), "Reset", font, &reset);

    buttonReset.create(Button::TYPE_PRESS, 0, (J_Window::getScreenHeight() - 14), "No", font, &no);
    buttonReset.create(Button::TYPE_PRESS, (J_Window::getScreenWidth() - ((font->getCharWidth() * 5))), (J_Window::getScreenHeight() - 14), "Yes", font, &yes);

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

        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 10) / 2), 0, "Highscores", grey);

        for (int i = 0, iy = 32; i < 10; ++i, iy += font->getCharHeight())
        {
            std::string score = std::to_string(Highscore::getScores()[i]);
            font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * (int)score.length()) / 2), iy, score, grey);
        }

        buttonMain.render();
    }
    else if (state == STATE_RESETTING)
    {
        J_Renderer::drawQuadFilled(background);

        J_Colour grey = { 83, 83, 83, 255, SDL_BLENDMODE_BLEND };

        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 10) / 2), 0, "Highscores", grey);

        int textX = (J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 32) / 2);
        int textY = (J_Window::getScreenHeight() / 2) - (font->getCharHeight() / 2);

        font->render(textX, textY, "Are you sure you want to reset?", grey);

        buttonReset.render();
    }
}



void Stat::menu (Button* a_button, const int a_interaction)
{
    state = STATE_INACTIVE;
}

void Stat::reset (Button* a_button, const int a_interaction)
{
    state = STATE_RESETTING;
}

void Stat::no (Button* a_button, const int a_interaction)
{
    state = STATE_ACTIVE;
}

void Stat::yes (Button* a_button, const int a_interaction)
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

    resetNoise.destroy();

    buttonMain.destroy();
    buttonReset.destroy();

    tick.destroy();

    state = -1;
}



void Stat::update ()
{
    // Nothing needed...
}

/// STAT //////////////////////////////////////////////////////////////////////
