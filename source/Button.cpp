/// BUTTON ////////////////////////////////////////////////////////////////////

const int Button::SPRITE_WIDTH = 4, Button::SPRITE_HEIGHT = 16;



void Button::create (const int a_x, const int a_y, const std::string a_text, J_Font* a_font, void (*a_process)())
{
    if (sprite != nullptr) { destroy(); }

    pos.point.x = a_x, pos.point.y = a_y;

    text = a_text;

    image.create("Button");

    for (int i = 0, ix = 0; i < SPRITE_TOTAL; ++i, ix += SPRITE_WIDTH)
    {
        sprite[i].quad = { ix, 0, SPRITE_WIDTH, SPRITE_HEIGHT };
    }

    font = a_font;

    process = a_process;

    selected = false;
}



void Button::render ()
{
    if (selected)
    {
        image.render(pos.point.x, pos.point.y, &sprite[SPRITE_LEFT]);

        for (int ix = (pos.point.x + SPRITE_WIDTH); ix < (pos.point.x + (SPRITE_WIDTH * (text.length() + 1))); ix += SPRITE_WIDTH)
        {
            image.render(ix, pos.point.y, &sprite[SPRITE_MIDDLE]);
        }

        image.render(pos.point.x + (SPRITE_WIDTH * (text.length() + 1)), pos.point.y, &sprite[SPRITE_RIGHT]);
    }

    J_Colour grey = { 83, 83, 83, 255, SDL_BLENDMODE_BLEND };
    font->render(pos.point.x + 4, pos.point.y + 4, text, grey);
}



void Button::press ()
{
    if (process != nullptr) { (*process)(); }
}



void Button::select ()
{
    selected = true;
}

void Button::deselect ()
{
    selected = false;
}



void Button::destroy ()
{
    pos.point = { 0, 0 };

    text = "\0";

    image.destroy();

    font = nullptr;

    process = nullptr;

    selected = false;
}

Button::~Button ()
{
    destroy();
}

/// BUTTON ////////////////////////////////////////////////////////////////////



/// BUTTON_LIST ///////////////////////////////////////////////////////////////

J_Sound ButtonList::sound;



void ButtonList::initialise ()
{
    sound.create("Collectible", 0);
}



void ButtonList::create (const int a_x, const int a_y, const std::string a_text, J_Font* a_font, void (*a_process)())
{
    button.push_back(nullptr);

    button.back() = new(std::nothrow) Button;
    if (button.back() == nullptr) { J_Error::log("GAME_ERROR_BUTTONLIST_ADD"); }

    button.back()->create(a_x, a_y, a_text, a_font, *a_process);

    position = 0;
    button.at(position)->select();
}



void ButtonList::render ()
{
    for (int i = 0; i < button.size(); ++i) { button.at(i)->render(); }
}



void ButtonList::forward ()
{
    if (!button.empty())
    {
        button.at(position)->deselect();

        ++position;
        if (position >= button.size()) { position = 0; }

        button.at(position)->select();

        // sound.play(0);
    }
}

void ButtonList::back ()
{
    if (!button.empty())
    {
        button.at(position)->deselect();

        --position;
        if (position < 0) { position = (button.size() - 1); }

        button.at(position)->select();

        // sound.play(0);
    }
}



void ButtonList::press ()
{
    if (!button.empty())
    {
        button.at(position)->press();
        sound.play(0);
    }
}



void ButtonList::terminate ()
{
    sound.destroy();
}



void ButtonList::destroy ()
{
    position = 0;

    if (!button.empty())
    {
        for (int i = 0; i < button.size(); ++i) { button.at(i)->destroy(); }
        button.clear();
    }
}

ButtonList::~ButtonList ()
{
    destroy();
}

/// BUTTON_LIST ///////////////////////////////////////////////////////////////
