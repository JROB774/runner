/// GAME //////////////////////////////////////////////////////////////////////

const std::string Game::HALLOWEEN_FILE = RES_DIR_DATA "Halloween.dat";
J_Background Game::front;
J_Background Game::back;
J_Collider Game::ground;
J_Font* Game::font = nullptr;
J_Font Game::bigFont;
J_Quad Game::bar;
int Game::score = 0, Game::bestScore = 0;
int Game::inputBuffer = 0;
bool Game::halloween = false;
int Game::state = Game::STATE_TERMINATE;



void Game::initialise (J_Font* a_font)
{
    std::ifstream file(HALLOWEEN_FILE);

    if (file.is_open())
    {
        file >> halloween;
        file.close();
    }
    else { J_Error::log("GAME_ERROR_LOAD_HALLOWEEN"); }

    front.create((halloween) ? "Halloween/Front" : "Front");
    back.create((halloween) ? "Halloween/Back" : "Back");

    ground.quad.quad = { 0, (J_Window::getScreenHeight() - (J_Window::getScreenHeight() / 6)),
                         J_Window::getScreenWidth(), (J_Window::getScreenHeight() / 6) };
    ground.quad.colour = { 0, 255, 0, 127, SDL_BLENDMODE_BLEND };

    font = a_font;
    bigFont.create("BigFont");

    bar.quad = { 0, ((J_Window::getScreenHeight() / 2) - font->getCharHeight()),
                 J_Window::getScreenWidth(), (font->getCharHeight() * 2) };
    bar.colour = { 83, 83, 83, 255, SDL_BLENDMODE_BLEND };

    score = 0;
    bestScore = Highscore::save(-1);

    Player::initialise(halloween);
    EntityHandler::initialise(halloween);
    Entity::setSpeed(3);

    inputBuffer = 2;

    state = STATE_ACTIVE;
}



void Game::handle ()
{
    if (state != STATE_TERMINATE)
    {
        if (state == STATE_ACTIVE) { Player::handle(); }

        if (state != STATE_INACTIVE)
        {
            if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
            {
                if (J_Input::getInput().key.keysym.sym == Player::getKey(Player::KEY_PAUSE))
                {
                    if (state != STATE_PAUSED)
                    {
                        front.stop();
                        back.stop();

                        state = STATE_PAUSED;
                    }
                    else if (state == STATE_PAUSED)
                    {
                        front.restart();
                        back.restart();

                        state = STATE_ACTIVE;
                    }
                }
            }
        }

        if (Player::isDead())
        {
            if (inputBuffer <= 0)
            {
                if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
                {
                    Game::terminate();
                }
            }
        }
    }
}



void Game::step ()
{
    if (state != STATE_TERMINATE)
    {
        if (state == STATE_ACTIVE)
        {
            Player::step(ground);
            if (EntityHandler::step(Player::getCollider())) { Player::kill(); }
            else { score += EntityHandler::getScore(); }

            if (Player::isDead())
            {
                front.stop();
                back.stop();

                Entity::setSpeed(0);

                state = STATE_INACTIVE;
            }
        }
        else if (state == STATE_INACTIVE) { if (inputBuffer > 0) { --inputBuffer; } }
    }
}



void Game::render ()
{
    if (state != STATE_TERMINATE)
    {
        back.render();
        front.render();

        EntityHandler::render();
        Player::render();

        ground.render();

        std::ostringstream stream;
        if (score <= bestScore) { stream << score; }
        else { stream << score << "!"; }
        bigFont.render((J_Window::getScreenWidth() / 2) - ((bigFont.getCharWidth() * (int)stream.str().length()) / 2), 0, stream.str(), bar.colour);

        if (state == STATE_PAUSED)
        {
            J_Renderer::drawQuadFilled(bar);
            font->render((J_Window::getScreenWidth() / 2) - (font->getCharWidth() * 6) / 2,
                         (J_Window::getScreenHeight() / 2) - (font->getCharHeight() / 2),
                         "Paused");
        }
        else if (state == STATE_INACTIVE)
        {
            J_Renderer::drawQuadFilled(bar);
            font->render((J_Window::getScreenWidth() / 2) - (font->getCharWidth() * 9) / 2,
                         (J_Window::getScreenHeight() / 2) - (font->getCharHeight() / 2),
                         "Game Over");
        }
    }
}



int Game::getScore ()
{
    return score;
}

int Game::getState ()
{
    return state;
}



void Game::terminate ()
{
    front.destroy();
    back.destroy();

    font = nullptr;
    bigFont.destroy();

    Player::terminate();
    EntityHandler::terminate();

    inputBuffer = 0;

    state = STATE_TERMINATE;
}

/// GAME //////////////////////////////////////////////////////////////////////
