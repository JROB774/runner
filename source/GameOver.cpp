/// GAME OVER /////////////////////////////////////////////////////////////////

J_Quad GameOver::background;
J_Image GameOver::medal;
J_Quad GameOver::sprite[GameOver::MEDAL_TOTAL];
J_Font* GameOver::font = nullptr;
J_Sound GameOver::scoreS, GameOver::medalS, GameOver::bestS;
int GameOver::score = 0, GameOver::finalScore = 0;
int GameOver::grade = 0;
int GameOver::startCounter = 0, GameOver::medalCounter = 0;
bool GameOver::newBest = false;
ButtonList GameOver::button;
J_Sound GameOver::tick;
int GameOver::state = -1;



void GameOver::initialise (J_Font* a_font, const int a_score)
{
    background.quad = { 0, 0, J_Window::getScreenWidth(), J_Window::getScreenHeight() };
    background.colour = { 255, 255, 255, 255, SDL_BLENDMODE_BLEND };

    medal.create(Game::getSeasonPath() + "Medal");

    for (int i = 0, ix = 0; i < MEDAL_TOTAL; ++i, ix += 40)
    {
        sprite[i].quad = { ix, 0, 40, 40 };
    }

    font = a_font;

    scoreS.create("Collectible", 0);
    medalS.create("Dead", 1);
    bestS.create("Best", 2);

    score = 0, finalScore = a_score;
    grade = MEDAL_EMPTY;

    if (Highscore::save(finalScore) == 1) { newBest = true; }
    else { newBest = false; }

    startCounter = 60;
    medalCounter = 60;

    button.create(Button::TYPE_PRESS, 0, (J_Window::getScreenHeight() - 14), "Restart", font, &restart);
    button.create(Button::TYPE_PRESS, (J_Window::getScreenWidth() - ((font->getCharWidth() * 6))), (J_Window::getScreenHeight() - 14), "Menu", font, &menu);

    tick.create("Tick", 3);

    state = STATE_ACTIVE;
}



void GameOver::handle ()
{
    if (state == STATE_ACTIVE)
    {
        if (startCounter <= 58)
        {
            if (score < finalScore)
            {
                if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
                {
                    score = finalScore;
                    startCounter = 0;
                }
            }
        }

        if (medalCounter <= 0)
        {
            if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
            {
                switch (J_Input::getInput().key.keysym.sym)
                {
                    case (SDLK_LEFT): { tick.play(0); button.back(); break; }
                    case (SDLK_RIGHT): { tick.play(0); button.forward(); break; }

                    case (SDLK_SPACE): { button.press(); break; }
                    case (SDLK_RETURN): { button.press(); break; }
                }
            }
        }

        if (medalCounter <= 58)
        {
            if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
            {
                medalCounter = 0;
            }
        }
    }
}



void GameOver::step ()
{
    if (state == STATE_ACTIVE)
    {
        if (startCounter <= 0)
        {
            if (score < finalScore)
            {
                ++score;
                scoreS.play(0);
            }

            if (score == finalScore)
            {
                if (medalCounter <= 0)
                {
                    if (grade == MEDAL_EMPTY)
                    {
                        if      (score >= 1000) { grade = MEDAL_A; }
                        else if (score >=  650) { grade = MEDAL_B; }
                        else if (score >=  400) { grade = MEDAL_C; }
                        else if (score >=  250) { grade = MEDAL_D; }
                        else if (score >=  100) { grade = MEDAL_E; }
                        else                    { grade = MEDAL_F; }

                        if (newBest) { bestS.play(0); }
                        else { medalS.play(0); }
                    }
                }
                else { --medalCounter; }
            }
        }
        else { --startCounter; }
    }
}



void GameOver::render ()
{
    if (state == STATE_ACTIVE)
    {
        J_Renderer::drawQuadFilled(background);

        medal.render((J_Window::getScreenWidth() / 2) - 20,
                     (J_Window::getScreenHeight() / 2) - 20,
                     &sprite[grade]);

        J_Colour grey = { 83, 83, 83, 255, SDL_BLENDMODE_BLEND };

        font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 9) / 2),
                     0, "Game Over", grey);

        if (startCounter <= 0)
        {
            std::ostringstream stream;
            stream << "Score: " << score;
            font->render((J_Window::getScreenWidth() / 2) - (((int)stream.str().length() * font->getCharWidth()) / 2),
                         (J_Window::getScreenHeight() / 2) + 20, stream.str(), grey);
        }

        if (medalCounter <= 0)
        {
            button.render();

            if (newBest)
            {
                font->render((J_Window::getScreenWidth() / 2) - ((font->getCharWidth() * 9) / 2),
                             (J_Window::getScreenHeight() / 2) + (20 + font->getCharHeight()),
                             "New Best!", grey);
            }
        }
    }
}



void GameOver::restart (Button* a_button, const int a_interaction)
{
    state = STATE_RESTART;
}

void GameOver::menu (Button* a_button, const int a_interaction)
{
    state = STATE_MENU;
}



int GameOver::getState ()
{
    return state;
}



void GameOver::terminate ()
{
    medal.destroy();

    font = nullptr;

    scoreS.destroy();
    medalS.destroy();
    bestS.destroy();

    score = 0, finalScore = 0;
    grade = 0;

    startCounter = 0;
    medalCounter = 0;

    newBest = false;

    button.destroy();

    tick.destroy();

    state = -1;
}

/// GAME OVER /////////////////////////////////////////////////////////////////
