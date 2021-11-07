#include "J_Input.h"

/// J_INPUT ///////////////////////////////////////////////////////////////////

const int J_Input::TOTAL_KEYS = 282;
const int J_Input::TOTAL_BUTTONS = 5;
bool* J_Input::key = nullptr;
bool* J_Input::button = nullptr;
SDL_Event* J_Input::input = nullptr;



void J_Input::initialise (SDL_Event* a_event)
{
    if (input != nullptr) { return; }

    input = a_event;

    key = new(std::nothrow) bool[TOTAL_KEYS];
    if (key == nullptr) { J_Error::log("J_ERROR_INPUT_KEY_ALLOCATE"); }
    for (int i = 0; i < TOTAL_KEYS; ++i) { key[i] = false; }

    button = new(std::nothrow) bool[TOTAL_BUTTONS];
    if (button == nullptr) { J_Error::log("J_ERROR_INPUT_BUTTON_ALLOCATE"); }
    for (int i = 0; i < TOTAL_BUTTONS; ++i) { button[i] = false; }
}



bool J_Input::keyPress (const int a_key)
{
    if (a_key <= -1) { return (input->type == SDL_KEYDOWN) ? true : false; }

    if (key != nullptr)
    {
        if (input->type == SDL_KEYDOWN)
        {
            if (input->key.keysym.sym == static_cast <SDL_Keycode> (a_key))
            {
                SDL_Scancode scancode = SDL_GetScancodeFromKey(static_cast <SDL_Keycode> (a_key));

                if (!key[scancode])
                {
                    key[scancode] = true;
                    return true;
                }
            }
        }
    }

    return false;
}

bool J_Input::keyRelease (const int a_key)
{
    if (a_key <= -1) { return (input->type == SDL_KEYUP) ? true : false; }

    if (key != nullptr)
    {
        if (input->type == SDL_KEYUP)
        {
            if (input->key.keysym.sym == static_cast <SDL_Keycode> (a_key))
            {
                SDL_Scancode scancode = SDL_GetScancodeFromKey(static_cast <SDL_Keycode> (a_key));

                if (key[scancode])
                {
                    key[scancode] = false;
                    return true;
                }
            }
        }
    }

    return false;
}



bool J_Input::mousePress (const int a_button)
{
    if (a_button <= -1) { return (input->type == SDL_MOUSEBUTTONDOWN) ? true : false; }

    if (button != nullptr)
    {
        if (input->type == SDL_MOUSEBUTTONDOWN)
        {
            if (input->button.button == a_button)
            {
                if (!button[a_button])
                {
                    button[a_button] = true;
                    return true;
                }
            }
        }
    }

    return false;
}

bool J_Input::mouseRelease (const int a_button)
{
    if (a_button <= -1) { return (input->type == SDL_MOUSEBUTTONUP) ? true : false; }

    if (button != nullptr)
    {
        if (input->type == SDL_MOUSEBUTTONUP)
        {
            if (input->button.button == a_button)
            {
                if (button[a_button])
                {
                    button[a_button] = false;
                    return true;
                }
            }
        }
    }

    return false;
}

void J_Input::mouseMotion (int* a_x, int* a_y)
{
    SDL_GetMouseState(a_x, a_y);
}



SDL_Event& J_Input::getInput ()
{
    return *input;
}



void J_Input::terminate ()
{
    if (input != nullptr)
    {
        input = nullptr;
        delete[] key;
        key = nullptr;
        delete[] button;
        button = nullptr;
    }
}

/// J_INPUT ///////////////////////////////////////////////////////////////////
