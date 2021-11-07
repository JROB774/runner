#include "Main.h"

#include "JEngine/J_Audio.cpp"
#include "JEngine/J_Render.cpp"
#include "JEngine/J_Collision.cpp"
#include "JEngine/J_Error.cpp"
#include "JEngine/J_Input.cpp"
#include "JEngine/J_Math.cpp"
#include "JEngine/J_Time.cpp"
#include "JEngine/J_System.cpp"
#include "JEngine/J_Window.cpp"

#include "Button.cpp"
#include "Container.cpp"
#include "Entity.cpp"
#include "EntityHandler.cpp"
#include "Game.cpp"
#include "GameOver.cpp"
#include "Highscore.cpp"
#include "Player.cpp"
#include "Menu.cpp"

int main (int argc, char* argv[])
{
    while (true)
    {
        J_System::initialise();
        Container::initialise();

        J_System::getFpsTimer()->start();

        while (J_System::getState() == J_System::STATE_ACTIVE)
        {
            J_System::getCapTimer()->start();

            while (J_System::pollEvent() != 0)
            {
                J_System::handle();
                Container::handle();
            }

            J_System::stepBegin();
            Container::step();
            Container::render();
            J_System::stepEnd();
        }

        Container::terminate();
        J_System::terminate();

        if (J_System::getState() == J_System::STATE_RESETTING) { continue; }
        else { break; }
    }

    if (J_System::isDebug()) { getchar(); }
    return 0;
}
