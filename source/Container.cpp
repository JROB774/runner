#include "Container.h"

/// CONTAINER /////////////////////////////////////////////////////////////////

J_Font Container::font;



void Container::initialise ()
{
    if (SDL_ShowCursor(SDL_DISABLE) < 0) { J_Error::log("GAME_ERROR_HIDE_CURSOR"); }

    font.create("Font");

    Menu::initialise(&font);
    ButtonList::initialise();

    srand(static_cast <unsigned int> (time(nullptr)));
}



void Container::handle ()
{
    /*
    if (J_Input::getInput().type == SDL_KEYDOWN)
    {
        if ((J_Input::getInput().key.keysym.sym == SDLK_ESCAPE) && (J_Input::getInput().key.repeat == 0))
        {
            J_System::stop();
        }
    }
    */

    if (Game::getState() != Game::STATE_TERMINATE)
    {
        Game::handle();

        if (Game::getState() == Game::STATE_TERMINATE)
        {
            GameOver::initialise(&font, Game::getScore());
            Game::terminate();
        }
    }

    if (GameOver::getState() == GameOver::STATE_ACTIVE)
    {
        GameOver::handle();

        if (GameOver::getState() != GameOver::STATE_ACTIVE)
        {
            if (GameOver::getState() == GameOver::STATE_RESTART) { Game::initialise(&font); }
            else if (GameOver::getState() == GameOver::STATE_MENU) { Menu::initialise(&font); }

            GameOver::terminate();
        }
    }

    if (Stat::getState() != Stat::STATE_INACTIVE)
    {
        Stat::handle();

        if (Stat::getState() == Stat::STATE_INACTIVE)
        {
            Menu::initialise(&font);
            Stat::terminate();
        }
    }

    if (Config::getState() != Config::STATE_INACTIVE)
    {
        Config::handle();

        if (Config::getState() == Config::STATE_INACTIVE)
        {
            Menu::initialise(&font);
            Config::terminate();
        }
    }

    if (Character::getState() != Character::STATE_INACTIVE)
    {
        Character::handle();

        if (Character::getState() == Character::STATE_INACTIVE)
        {
            Menu::initialise(&font);
            Character::terminate();
        }
    }

    if (Menu::getState() == Menu::STATE_ACTIVE)
    {
        Menu::handle();

        switch (Menu::getState())
        {
            case (Menu::STATE_PLAY):
            {
                Game::initialise(&font);
                Menu::terminate();

                break;
            }

            case (Menu::STATE_CHARACTER):
            {
                Character::initialise(&font);
                Menu::terminate();

                break;
            }

            case (Menu::STATE_STATS):
            {
                Stat::initialise(&font);
                Menu::terminate();

                break;
            }

            case (Menu::STATE_CONFIG):
            {
                Config::initialise(&font);
                Menu::terminate();

                break;
            }
        }
    }
}



void Container::step ()
{
    Menu::step();

    Game::step();
    GameOver::step();
}



void Container::render ()
{
    Menu::render();
    Character::render();
    Stat::render();
    Config::render();

    Game::render();
    GameOver::render();
}



void Container::terminate ()
{
    font.destroy();

    ButtonList::terminate();
    Menu::terminate();
    Character::terminate();
    Stat::terminate();
    Config::terminate();
    GameOver::terminate();
    Game::terminate();
}

/// CONTAINER /////////////////////////////////////////////////////////////////
